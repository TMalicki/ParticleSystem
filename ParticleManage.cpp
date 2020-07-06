#include "ParticleManage.h"
#include <algorithm>

using std::vector;

void ParticleManage::createParticles(std::vector<std::shared_ptr<ParticlesInterface>>& particles, sf::Vector2i mousePosition, int amount)
{
	auto mousePositionFloat = sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

	if (m_type == ParticleType::Vertex)
	{
		particles.push_back(std::shared_ptr<ParticlesInterface>(new ParticlesVertex(amount, sf::Vector2f(mousePositionFloat.x, mousePositionFloat.y))));
	}
	else if (m_type == ParticleType::CircleShape)
	{
		particles.push_back(std::shared_ptr<ParticlesInterface>(new ParticlesCircle(amount, sf::Vector2f(mousePositionFloat.x, mousePositionFloat.y))));
	}
}

void ParticleManage::createParticles(std::vector<std::shared_ptr<ParticlesInterface>>& particles, std::vector<sf::Vector2i> mousePosition, int amount)
{
	std::vector<sf::Vector2f> mousePositionFloat{ mousePosition.size() };
	int index{};

	for (auto& floatPosition : mousePosition)
	{
		mousePositionFloat[index] = sf::Vector2f(static_cast<float>(floatPosition.x), static_cast<float>(floatPosition.y));
		index++;
	}

	if (m_type == ParticleType::Vertex)
	{
		particles.push_back(std::shared_ptr<ParticlesInterface>(new ParticlesVertex(amount, mousePositionFloat)));
	}
	else if (m_type == ParticleType::CircleShape)
	{
		particles.push_back(std::shared_ptr<ParticlesInterface>(new ParticlesCircle(amount, mousePositionFloat)));
	}
}

void ParticleManage::applyEffect(ParticleEffect effect, sf::Vector2i mousePosition, sf::Vector2f forceRange, sf::Vector2f angleRange, int amount) 
{
	if (effect == ParticleEffect::Explode)
	{
		createParticles(m_particleContainer[0], mousePosition, amount);
		//angleRange = sf::Vector2f(0.0f, 2.0f * 3.14f);
		createForceWave(mousePosition);
	}
	else if (effect == ParticleEffect::Emiter)
	{
		createParticles(m_particleContainer[1], mousePosition, amount);
		//angleRange = sf::Vector2f(250.0f * 0.0174f, 290.0f * 0.0174f); // (pi / 180 degree) == 0,0174
	}
		
	vector<sf::Vector2f> directionVector(amount);

	std::for_each(directionVector.begin(), directionVector.end(), [&](sf::Vector2f& direction)
	{
		float randomDirection = getRandomFloat(angleRange.x, angleRange.y);
		direction = sf::Vector2f(static_cast<float>(cos(randomDirection)), static_cast<float>(sin(randomDirection)));
	});

	if (effect == ParticleEffect::Explode) setParticleExpandAttributes(m_particleContainer[0], mousePosition, directionVector, forceRange);
	else if (effect == ParticleEffect::Emiter) setParticleExpandAttributes(m_particleContainer[1], mousePosition, directionVector, forceRange);
}

void ParticleManage::applyEffect(ParticleEffect effect, std::vector<sf::Vector2i> mousePosition, sf::Vector2f forceRange, sf::Vector2f angleRange, int amount)
{
	if (effect == ParticleEffect::Tunnel)
	{
		createParticles(m_particleContainer[2], mousePosition, amount);
		// apply force / direction so it goes only outside of circle
	}

	vector<sf::Vector2f> directionVector(amount);

	std::for_each(directionVector.begin(), directionVector.end(), [&](sf::Vector2f& direction)
	{
		float randomDirection = getRandomFloat(angleRange.x, angleRange.y);
		direction = sf::Vector2f(static_cast<float>(cos(randomDirection)), static_cast<float>(sin(randomDirection)));
	});

	if (effect == ParticleEffect::Tunnel)
	{
		for (size_t i = 0; i < amount; i++)
		{
			directionVector[i] = sf::Vector2f{ cos(i * 3.14f / 180), sin(i * 3.14f / 180) };
		}
		setParticleExpandAttributes(m_particleContainer[2], mousePosition, directionVector, forceRange);
	}
}

void ParticleManage::TurnEffectOn(ParticleEffect effect)
{
	if (effect == ParticleEffect::Emiter) m_emiterParticlesOn = true;
	else if (effect == ParticleEffect::Explode) m_explodedParticlesOn = true;
	else if (effect == ParticleEffect::Tunnel) m_tunnelParticlesOn = true;
}

void ParticleManage::createEmitingObject(sf::Vector2i mousePosition, float spawnFrequency, int amount)
{
	if (m_activeArea.x > mousePosition.x)
	{
		emiterEffect.createEmiter(mousePosition, spawnFrequency, amount);
	}
}

std::vector<size_t> ParticleManage::eraseParticles(std::vector<std::shared_ptr<ParticlesInterface>>& particles, std::vector<std::vector<size_t>> elementsID)	
{
	std::vector<size_t> toEraseGroup{};

	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i]->eraseParticles(elementsID[i]);	

		if (particles[i]->getParticlesAmount() == 0)
		{
			toEraseGroup.push_back(i);
		}
	}
	return toEraseGroup;
}

void ParticleManage::eraseParticlesGroup(std::vector<std::shared_ptr<ParticlesInterface>>& particles, std::vector<size_t> toEraseGroup)
{
	std::sort(toEraseGroup.begin(), toEraseGroup.end(), std::greater<size_t>());

	for (auto erase : toEraseGroup)
	{
		particles.at(erase) = particles.back();
		particles.pop_back();
	}
}

void ParticleManage::updateFading(float dt)
{
	if (m_fadingOn == true)
	{
		static float sum{};
		static float maxLifeTime = ParticlesInterface::getMaxLifeTime();

		sum += dt;

		for (auto& particleContainer : m_particleContainer)
		{
			std::for_each(particleContainer.begin(), particleContainer.end(), [&](auto& particle) { particle->reduceLifeTime(dt, sum, maxLifeTime); });
		}
		if (sum >= (maxLifeTime / 255.0f))
		{
			sum = 0.0f;
		}
	}
}

void ParticleManage::colorParticlesByVelocity(std::vector<std::shared_ptr<ParticlesInterface>>& particles)
{
	// for_each(particles.begin(), particles.end(), []{
	if (particles.size() > 0)
	{
		auto maxVelocity = ParticlesInterface::getMaxVelocity();
		for (auto& particle : particles)
		{
			auto tempVelocities = particle->getVelocity();
			auto tempColor = particle->getColor();

			for (size_t i = 0; i < particle->getParticlesAmount(); i++)
			{
				volatile int calculatedRGB = 255 - static_cast<int>((sqrt(pow(tempVelocities[i].x, 2) + pow(tempVelocities[i].y, 2)) / maxVelocity) * 380.0f);	///380, not 255 for faster red color achieved
				if (calculatedRGB >= 255) calculatedRGB = 255;
				tempColor[i] = sf::Color(255, calculatedRGB, calculatedRGB, tempColor[i].a);
			}
			particle->setColor(tempColor); 
		}
	}
}

void ParticleManage::applyFading(bool logic)
{
	m_fadingOn = logic;
}

void ParticleManage::setParticleExpandAttributes(vector<std::shared_ptr<ParticlesInterface>>& particleGroup, sf::Vector2i mousePosition, vector<sf::Vector2f> direction, sf::Vector2f randomRange)
{
	auto& actualParticleGroup = particleGroup.back();
	auto& actualParticleGroupAttributes = actualParticleGroup->getParticleAttributes();

	// random forces, and random masses
	vector<sf::Vector2f> randomForces(actualParticleGroupAttributes.size());
	vector<float> randomMasses(actualParticleGroupAttributes.size());

	for (size_t i = 0; i < actualParticleGroupAttributes.size(); i++)
	{
		float random_x = getRandomFloat(randomRange.x, randomRange.y);
		float random_y = getRandomFloat(randomRange.x, randomRange.y);

		auto tempForce = sf::Vector2f(random_x, random_y);
		randomForces.at(i) = tempForce;

		//sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
		
		randomMasses.at(i) = getRandomFloat(1.5f, 3.0f);
	}
	actualParticleGroup->setMass(randomMasses);
	actualParticleGroup->setDirection(direction);
	actualParticleGroup->applyForce(randomForces);
}

void ParticleManage::setParticleExpandAttributes(std::vector<std::shared_ptr<ParticlesInterface>>& particleGroup, std::vector<sf::Vector2i> mousePosition, std::vector<sf::Vector2f> direction, sf::Vector2f randomRange)
{
	auto& actualParticleGroup = particleGroup.back();
	auto& actualParticleGroupAttributes = actualParticleGroup->getParticleAttributes();

	// random forces, and random masses
	vector<sf::Vector2f> randomForces(actualParticleGroupAttributes.size());
	vector<float> randomMasses(actualParticleGroupAttributes.size());

	for (size_t i = 0; i < actualParticleGroupAttributes.size(); i++)
	{
		float random_x = getRandomFloat(randomRange.x, randomRange.y);
		float random_y = getRandomFloat(randomRange.x, randomRange.y);

		auto tempForce = sf::Vector2f(random_x, random_y);
		randomForces.at(i) = tempForce;

		//sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

		randomMasses.at(i) = getRandomFloat(1.5f, 3.0f);
	}
	actualParticleGroup->setMass(randomMasses);
	actualParticleGroup->setDirection(direction);
	actualParticleGroup->applyForce(randomForces);
}


void ParticleManage::createForceWave(sf::Vector2i mousePosition, float radius)
{
	m_force.push_back(sf::CircleShape(radius,100));
	m_force.back().setOutlineThickness(-1.0f);
	m_force.back().setOrigin(radius, radius);
	m_force.back().setPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
	m_force.back().setFillColor(sf::Color::Transparent);
	m_force.back().setOutlineColor(sf::Color::White);
	setWaveForce(0.5f);
}

void ParticleManage::forceWaveExpand(float velocity, sf::Vector2f windowSize)
{
	vector<size_t> toDestroy{};

	for (size_t i = 0; i < m_force.size(); i++)
	{
		m_force[i].setRadius(m_force[i].getRadius() + velocity);
		m_force[i].setOrigin(m_force[i].getRadius(), m_force[i].getRadius());

		auto forceRadius = m_force[i].getRadius();
		auto forceWidth = m_force[i].getPosition().x;

		if (forceWidth >= windowSize.x / 2)
		{
			if (forceRadius >= forceWidth)
			{
				toDestroy.push_back(i);
			}
		}
		else if (forceWidth < windowSize.x / 2)
		{
			if (forceRadius >= windowSize.x - forceWidth)
			{
				toDestroy.push_back(i);
			}
		}
	}
	
	std::sort(toDestroy.begin(), toDestroy.end(), std::greater<size_t>());
	for (auto destroy : toDestroy)
	{
		m_force.at(destroy) = m_force.back();
		m_force.pop_back();
	}
}


const auto ParticleManage::isForceWaveCollided()
{
	bool collision = false;
	vector<std::tuple<size_t, size_t, size_t>> indexes{};

	for (size_t k = 0; k < m_force.size(); k++)
	{
		auto actualForcePosition = m_force[k].getPosition();
		auto actualForceRadius = m_force[k].getRadius();

		for (size_t i = 0; i < m_particleContainer[0].size(); i++)
		{
			 if(!((i == m_particleContainer[0].size() - 1) && (k == m_force.size() - 1)))
			{
				auto size = m_particleContainer[0][i]->getParticlesAmount();
				auto positionsVector = m_particleContainer[0].at(i)->getPosition();

				for (size_t j = 0; j < size; j++)
				{
					auto positionFromForceWave = positionsVector.at(j) - actualForcePosition;	// here is problem
					auto distanceFromForceWave = sqrt(pow(positionFromForceWave.x, 2) + pow(positionFromForceWave.y, 2));

					if ((distanceFromForceWave - actualForceRadius > -40.0f) && (distanceFromForceWave - actualForceRadius) <= 20.0f)
					{
						indexes.push_back({ i, j, k });
					}
				}
			}
		}
	}
	if(indexes.size()>0) collision = true;

	return std::make_pair(collision,indexes);
}

void ParticleManage::setForceRange(sf::Vector2f forceRange)
{
	m_forceRange = forceRange;
}

void ParticleManage::particlePush(const vector<std::tuple<size_t, size_t, size_t>>& pushedParticlesIndex, bool collision)
{
	if (collision == true)
	{
		//std::vector<std::tuple<size_t,sf::Vector2f>> tempPositions;
		//std::vector<std::tuple<size_t, sf::Vector2f>> tempDirections;
		//std::vector<size_t> tempIndexes;

		//tempIndexes.reserve(100);
		//tempPositions.reserve(10000);
		//tempDirections.reserve(10000);

		// update of already existing particle groups
		for (auto pushedParticleIndex : pushedParticlesIndex)
		{
			auto [particleGroup, particleInGroup, forceWave] = pushedParticleIndex;

			//tempIndexes.push_back(particleGroup);
			auto& actualParticleAttribute = m_particleContainer[0].at(particleGroup)->getParticleAttributes().at(particleInGroup);
			auto actualPosition = m_particleContainer[0].at(particleGroup)->getPosition().at(particleInGroup);
			
			auto newDirectionVector = sf::Vector2f{ (actualPosition.x - m_force[forceWave].getPosition().x), (actualPosition.y - m_force[forceWave].getPosition().y) };
			auto newDirectionMagnitude = abs(newDirectionVector.x) + abs(newDirectionVector.y);
			auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };
			
			//tempDirections.push_back(std::make_tuple(particleInGroup, newDirection));
			//tempPositions.push_back(std::make_tuple(particleInGroup, sf::Vector2f{ m_forceWaveForce, m_forceWaveForce }));
			

			m_particleContainer[0].at(particleGroup)->setDirection(particleInGroup, newDirection);
			m_particleContainer[0].at(particleGroup)->applyForce(particleInGroup, sf::Vector2f{ m_forceWaveForce ,m_forceWaveForce });
		}
		/*
		for (auto index : tempIndexes)
		{
			m_explodedParticles.at(index)->setDirection(tempDirections);
			m_explodedParticles.at(index)->applyForce(tempPositions);
		}
		*/
	}
	bool t = false;
}


void ParticleManage::TurnOnForce(bool logic, ParticleSettings::Forces force)
{
	if (force == ParticleSettings::Forces::Gravity)
	{
		m_GravityOn = logic;
	}
	else if (force == ParticleSettings::Forces::AirResistance)
	{
		m_AirResistanceOn = logic;
	}
	else if (force == ParticleSettings::Forces::Friction)
	{
		m_FrictionOn = logic;
	}
	else if (force == ParticleSettings::Forces::Wind)
	{
		m_WindOn = logic;
	}
}

void ParticleManage::updateForce()
{
	if (m_GravityOn == true) applyGravityForce(sf::Vector2f{ 0.0f,0.02f });
	if (m_AirResistanceOn == true) applyAirResistance();
	if (m_FrictionOn == true) applyFriction();
	if (m_WindOn == true)
	{
		for (auto& particleContainer : m_particleContainer)
		{
			for (auto& particlesGroup : particleContainer)
			{
				particlesGroup->setDirection(m_WindDirection);
			}
		}
		applyWindForce(sf::Vector2f{ 0.02f,0.02f });
	}
}

void ParticleManage::updatePosition(float dt)
{
	for (auto& particleContainer : m_particleContainer)
	{
		for_each(particleContainer.begin(), particleContainer.end(), [&](std::shared_ptr<ParticlesInterface>& particles) {particles->update(dt * 2.0f); });
	}
}

void ParticleManage::applyWindForce(sf::Vector2f force)
{
	for (auto& particleContainer : m_particleContainer)
	{
		for_each(particleContainer.begin(), particleContainer.end(), [&](std::shared_ptr<ParticlesInterface>& particles) { particles->applyForce(force); });
	}
}

void ParticleManage::applyGravityForce(sf::Vector2f force)
{
	for (auto& particleContainer : m_particleContainer)
	{
		for_each(particleContainer.begin(), particleContainer.end(), [&](std::shared_ptr<ParticlesInterface>& particles) { particles->applyGravityForce(force); });
	}
}

void ParticleManage::applyAirResistance(float coefficent)
{
	for (auto& particleContainer : m_particleContainer)
	{
		for_each(particleContainer.begin(), particleContainer.end(), [&](auto& particles) { particles->applyAirResistance(coefficent); });
	}
}

void ParticleManage::applyFriction(float mi)
{
	for (auto& particleContainer : m_particleContainer)
	{
		for_each(particleContainer.begin(), particleContainer.end(), [&](std::shared_ptr<ParticlesInterface>& particles) { particles->applyFriction(mi); });
	}
}

sf::String ParticleManage::getEffectText()
{
	if (m_effectType == ParticleEffect::Emiter) return "EMITER";
	else if (m_effectType == ParticleEffect::Explode) return "EXPLODE";
	else if (m_effectType == ParticleEffect::Tunnel) return "TUNNEL";
	else return "NOTHING";
}

void ParticleManage::update(float dt, sf::Vector2i mousePosition, int particleAmount)
{
	// inside those methods are effect containers like m_explodedParticles
	// if new effect (and container for that effect) is added update also
	// those methods
	updateFading(dt);	
	updateForce();
	updatePosition(dt);
	///////////////////////////////////////////////////////////////////////
	
	emiterEffect.updateEmiter(dt);
	
	if (m_explodedParticlesOn == true)
	{
		applyEffect(ParticleManage::ParticleEffect::Explode, mousePosition, getForceRange(), sf::Vector2f(0.0f, 2.0f * 3.14f), particleAmount);
		m_explodedParticlesOn = false;
	}
	
	if (emiterEffect.getEmiterLogic() == true)
	{
		//createEmitingObject(mousePosition, 20.0f, particleAmount);
		auto emiterPositions = emiterEffect.getEmitersPositions();
		for (auto& emiterPos : emiterPositions)
		{
			applyEffect(ParticleManage::ParticleEffect::Emiter, sf::Vector2i{ static_cast<int>(emiterPos.x),static_cast<int>(emiterPos.y) }, 
																sf::Vector2f(m_forceRange.x, m_forceRange.y), sf::Vector2f(250.0f * 0.0174f, 290.0f * 0.0174f), 1);
		}
		emiterEffect.setEmiterLogic(false);
	}
	if (m_tunnelParticlesOn == true)
	{
		std::vector<sf::Vector2i> point{ 360 };
		for (size_t angle = 0; angle < 360; angle++)
		{
			point[angle] = sf::Vector2i{ mousePosition.x + static_cast<int>(100 * cos(angle * 3.14f / 180.0f)),
											   mousePosition.y + static_cast<int>(100 * sin(angle * 3.14f / 180.0f)) };
		}
			applyEffect(ParticleManage::ParticleEffect::Tunnel, point, sf::Vector2f{ getForceRange().y, getForceRange().y }, sf::Vector2f(0.0f, 2.0f * 3.14f), 360);	
	}

	forceWaveExpand(getWaveForce() * dt, m_activeArea);
	auto particlesPushed = isForceWaveCollided();
	particlePush(particlesPushed.second, particlesPushed.first);
}



void ParticleManage::draw(sf::RenderWindow& window)
{
	emiterEffect.draw(window);

	for (auto& particleContainer : m_particleContainer)
	{
		for_each(particleContainer.begin(), particleContainer.end(), [&](auto& particles) { particles->draw(window); });
	}
	
	for_each(m_force.begin(), m_force.end(), [&](const auto& force) { window.draw(force); });
}

float ParticleManage::getRandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	float temp = distribution(m_generator);

	return temp;
}
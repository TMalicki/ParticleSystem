#include "ParticleManage.h"
#include <algorithm>

using std::vector;

void ParticleManage::createParticles(std::vector<std::shared_ptr<ParticlesInterface>>& particles, sf::Vector2i mousePosition, int amount)
{
	auto mousePositionFloat = sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
	// here reserve should be added to optimize that section
	if (m_type == ParticleType::Vertex)
	{
		particles.push_back(std::shared_ptr<ParticlesInterface>(new ParticlesVertex(amount, sf::Vector2f(mousePositionFloat.x, mousePositionFloat.y))));
	}
	else if (m_type == ParticleType::CircleShape)
	{
		particles.push_back(std::shared_ptr<ParticlesInterface>(new ParticlesCircle(amount, sf::Vector2f(mousePositionFloat.x, mousePositionFloat.y))));
	}
}

void ParticleManage::createEmitingObject(sf::Vector2i mousePosition)
{
	if (m_activeArea.x > mousePosition.x)
	{
		emiterEffect.createEmiter(mousePosition);
	}
}

void ParticleManage::applyEffect(ParticleEffect effect, sf::Vector2i mousePosition, sf::Vector2f forceRange, int amount)
{
	if (m_activeArea.x > mousePosition.x)
	{
		sf::Vector2f angleRange{};
		if (effect == ParticleEffect::Explode)
		{
			createParticles(m_explodedParticles, mousePosition, amount);
			angleRange = sf::Vector2f(0.0f, 2.0f * 3.14f);
			createForceWave(mousePosition);
		}
		else if (effect == ParticleEffect::Emiter)
		{
			createParticles(m_emiterParticles, mousePosition, amount);
			angleRange = sf::Vector2f(230.0f * 0.0174f, 310.0f * 0.0174f); // (pi / 180 degree) == 0,0174
		}
		vector<sf::Vector2f> directionVector(amount);

		std::for_each(directionVector.begin(), directionVector.end(), [&](sf::Vector2f& direction)
		{
			float randomDirection = getRandomFloat(angleRange.x, angleRange.y);
			direction = sf::Vector2f(static_cast<float>(cos(randomDirection)), static_cast<float>(sin(randomDirection)));
		});

		if (effect == ParticleEffect::Explode) setParticleExpandAttributes(m_explodedParticles, mousePosition, directionVector, forceRange);
		else if (effect == ParticleEffect::Emiter) setParticleExpandAttributes(m_emiterParticles, mousePosition, directionVector, forceRange);
	}
}

void ParticleManage::updateFading(std::vector<std::shared_ptr<ParticlesInterface>>& particlesGroup, float dt)
{
	static float sum{};
	static float maxLifeTime = ParticlesInterface::getMaxLifeTime();

	sum += dt;

	for (auto& particle : particlesGroup)
	{
		particle->reduceLifeTime(dt);
		
		if (sum >= (maxLifeTime / 255.0f))
		{
			particle->reduceColorOpacity(1);
		}
		particle->toErase();
	}
	if (sum >= (maxLifeTime / 255.0f))
	{
		sum = 0.0f;
	}
}

void ParticleManage::colorParticlesByVelocity(std::vector<std::shared_ptr<ParticlesInterface>>& particles)
{
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
				tempColor[i] = sf::Color(255, calculatedRGB, calculatedRGB, tempColor[i].a);// here is problem, why?
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

		sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
		
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
	// to powinna byc przekatna a nie bok prostokąta
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

/// here is something wrong
const auto ParticleManage::isForceWaveCollided()
{
	bool collision = false;
	vector<std::tuple<size_t, size_t, size_t>> indexes{};
//	std::cout << "1\n";
	for (size_t k = 0; k < m_force.size(); k++)
	{
		auto actualForcePosition = m_force[k].getPosition();
		auto actualForceRadius = m_force[k].getRadius();
	//	std::cout << "2\n";
		for (size_t i = 0; i < m_explodedParticles.size(); i++)
		{
			 if(!((i == m_explodedParticles.size() - 1) && (k == m_force.size() - 1)))
			{
				auto size = m_explodedParticles[i]->getParticlesAmount();
				auto positionsVector = m_explodedParticles.at(i)->getPosition(); 

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
//		std::cout << "3\n";
	}
	if(indexes.size()>0) collision = true;

	return std::make_pair(collision,indexes);
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
			auto& actualParticleAttribute = m_explodedParticles.at(particleGroup)->getParticleAttributes().at(particleInGroup);
			auto actualPosition = m_explodedParticles.at(particleGroup)->getPosition().at(particleInGroup);
			
			auto newDirectionVector = sf::Vector2f{ (actualPosition.x - m_force[forceWave].getPosition().x), (actualPosition.y - m_force[forceWave].getPosition().y) };
			auto newDirectionMagnitude = abs(newDirectionVector.x) + abs(newDirectionVector.y);
			auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };
			
			//tempDirections.push_back(std::make_tuple(particleInGroup, newDirection));
			//tempPositions.push_back(std::make_tuple(particleInGroup, sf::Vector2f{ m_forceWaveForce, m_forceWaveForce }));
			
			m_explodedParticles.at(particleGroup)->setDirection(particleInGroup, newDirection);
			m_explodedParticles.at(particleGroup)->applyForce(particleInGroup, sf::Vector2f{ m_forceWaveForce ,m_forceWaveForce });
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

void ParticleManage::forceUpdate()
{
	if (m_GravityOn == true) applyGravityForce(sf::Vector2f{ 0.0f,0.02f });
	if (m_AirResistanceOn == true) applyAirResistance();
	if (m_FrictionOn == true) applyFriction();
	if (m_WindOn == true)
	{
		for (auto& particlesGroup : m_explodedParticles)
		{
			particlesGroup->setDirection(m_WindDirection);
		}
		for (auto& particlesGroup : m_emiterParticles)
		{
			particlesGroup->setDirection(m_WindDirection);
		}
		applyWindForce(sf::Vector2f{ 0.02f,0.02f });
	}
}

void ParticleManage::applyWindForce(sf::Vector2f force)
{
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::shared_ptr<ParticlesInterface>& particles) {particles->applyForce(force); });
	std::for_each(m_emiterParticles.begin(), m_emiterParticles.end(), [&](std::shared_ptr<ParticlesInterface>& particles) {particles->applyForce(force); });
}

void ParticleManage::applyGravityForce(sf::Vector2f force)
{
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::shared_ptr<ParticlesInterface>& particles) {particles->applyGravityForce(force); });
	std::for_each(m_emiterParticles.begin(), m_emiterParticles.end(), [&](std::shared_ptr<ParticlesInterface>& particles) {particles->applyGravityForce(force); });
}

void ParticleManage::applyAirResistance(float coefficent)
{
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::shared_ptr<ParticlesInterface>& particles) {particles->applyAirResistance(coefficent); });
	std::for_each(m_emiterParticles.begin(), m_emiterParticles.end(), [&](std::shared_ptr<ParticlesInterface>& particles) {particles->applyAirResistance(coefficent); });
}

void ParticleManage::applyFriction(float mi)
{
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::shared_ptr<ParticlesInterface>& particles) {particles->applyFriction(mi); });
	std::for_each(m_emiterParticles.begin(), m_emiterParticles.end(), [&](std::shared_ptr<ParticlesInterface>& particles) {particles->applyFriction(mi); });
}

sf::String ParticleManage::getEffectText()
{
	if (m_effectType == ParticleEffect::Emiter) return "EMITER";
	else if (m_effectType == ParticleEffect::Explode) return "EXPLODE";
	else return "NOTHING";
}

void ParticleManage::update(float dt)
{
	if (m_fadingOn == true)
	{
		updateFading(m_explodedParticles, dt);
		updateFading(m_emiterParticles, dt);
	}
	forceUpdate();
	// for moving
	for (auto& particleGroup : m_explodedParticles)
	{
		particleGroup->update(dt* 2.0f);
	}

	/*// fire 
	for (size_t i = 0; i < m_EmiterObject.size(); i++)
	{
		auto mousePosition = m_EmiterObject[i].getPosition();

		emitter(sf::Vector2i{ static_cast<int>(mousePosition.x),static_cast<int>(mousePosition.y) }, sf::Vector2f(-3.0, 3.0), 1);
		m_EmiterCounter[i] += 1;
		if (m_EmiterCounter[i] >= 10)
		{
			m_EmiterCounter[i] = 0;
			m_EmiterOn = false;
		}
	}
	*/
	emiterEffect.updateEmiter(dt);
	if (emiterEffect.getEmiterLogic() == true)
	{
		auto emiterPositions = emiterEffect.getEmitersPositions();
		for (auto& emiterPos : emiterPositions)
		{
			applyEffect(ParticleManage::ParticleEffect::Emiter, sf::Vector2i{ static_cast<int>(emiterPos.x),static_cast<int>(emiterPos.y) }, sf::Vector2f(-3.0, 3.0), 1);
			//emitter(sf::Vector2i{ static_cast<int>(emiterPos.x),static_cast<int>(emiterPos.y) }, sf::Vector2f(-3.0, 3.0), 1);
		}
		emiterEffect.setEmiterLogic(false);
	}

	for (auto& particleGroup : m_emiterParticles)
	{
		particleGroup->update(dt * 2.0f);
	}
	forceWaveExpand(getWaveForce() * dt, m_activeArea);

	auto particlesPushed = isForceWaveCollided();
	particlePush(particlesPushed.second, particlesPushed.first);
}

void ParticleManage::draw(sf::RenderWindow& window)
{
	emiterEffect.draw(window);
	for (const auto& particleGroup : m_explodedParticles)
	{
		particleGroup->draw(window);
	}
	for (const auto& particleGroup : m_emiterParticles)
	{
		particleGroup->draw(window);
	}
	for (const auto& force : m_force)
	{
		window.draw(force);
	}
}

float ParticleManage::getRandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	float temp = distribution(m_generator);

	return temp;
}
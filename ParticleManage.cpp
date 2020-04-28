#include "ParticleManage.h"
#include <algorithm>

using std::vector;

void ParticleManage::createParticles(sf::PrimitiveType type, sf::Vector2i mousePosition, int amount)
{
	m_explodedParticles.push_back(std::unique_ptr<Particles>(new Particles(amount, sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)), sf::Vector2f(0.0, 0.0), sf::Vector2f(0.0, 0.0), type)));
	//std::sort(m_explodedParticles.begin(),m_explodedParticles.end(),)
}

void ParticleManage::explode(sf::Vector2i mousePosition, sf::PrimitiveType type, sf::Vector2f randomRange, int amount)
{
	createParticles(type, mousePosition, amount);
	setParticleExpandAttributes(m_explodedParticles, mousePosition, type, randomRange, amount);

	createForceWave(mousePosition);
}

void ParticleManage::setParticleExpandAttributes(vector<std::unique_ptr<Particles>>& particleGroup, sf::Vector2i mousePosition, sf::PrimitiveType type, sf::Vector2f randomRange, int amount)
{
	auto& actualParticleGroup = particleGroup.back();
	auto& actualParticleGroupAttributes = actualParticleGroup->getParticleAttributes();

	// random forces, and random masses
	vector<sf::Vector2f> randomDirections(actualParticleGroupAttributes.size());
	vector<sf::Vector2f> randomForces(actualParticleGroupAttributes.size());
	vector<float> randomMasses(actualParticleGroupAttributes.size());

	for (size_t i = 0; i < actualParticleGroup->getParticleVertex().size(); i++)
	{
		float random_x = getRandomFloat(randomRange.x, randomRange.y);
		float random_y = getRandomFloat(randomRange.x, randomRange.y);

		auto tempForce = sf::Vector2f(random_x, random_y);
		randomForces.at(i) = tempForce;

		sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
		randomDirections.at(i) = sf::Vector2f(static_cast<float>(cos(i)), static_cast<float>(sin(i)));

		randomMasses.at(i) = getRandomFloat(1.0f, 3.0f);
	}

	actualParticleGroup->setMass(randomMasses);
	actualParticleGroup->setDirection(randomDirections);
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
	setForceVelocity(0.2f);
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
	
	for (auto destroy : toDestroy)
	{
		m_force.erase(m_force.begin() + destroy);
	}
}

const auto ParticleManage::isForceWaveCollided()
{
	// czasteczki ktore wychodza poza ekran powinny byc w zaleznosci od trybu usuwane, odbijane od krawedzi, lub powinny wychodzic z innej strony ekranu 

	bool collision = false;
	vector<std::tuple<size_t, size_t, size_t>> indexes{};
	
	for (size_t k = 0; k < m_force.size(); k++)
	{
		auto actualForcePosition = m_force[k].getPosition();
		auto actualForceRadius = m_force[k].getRadius();

		for (size_t i = 0; i < m_explodedParticles.size(); i++)
		{
			 if(!((i == m_explodedParticles.size() - 1) && (k == m_force.size() - 1)))
			{
				auto actualParticleGroup = m_explodedParticles[i]->getParticleVertex();

				for (size_t j = 0; j < actualParticleGroup.size(); j++)
				{
					auto positionFromForceWave = (actualParticleGroup[j].position - actualForcePosition);
					auto distanceFromForceWave = sqrt(pow(positionFromForceWave.x, 2) + pow(positionFromForceWave.y, 2));

					if ((distanceFromForceWave - actualForceRadius > -40.0f) && (distanceFromForceWave - actualForceRadius) <= 20.0f)
					{
						indexes.push_back({ i,j, k });
						m_explodedParticles[i]->setParticleColor(j, sf::Color::Green);
					}
				}
			}
		}
	}
	if(indexes.size()>0) collision = true;

	return std::make_pair(collision,indexes);
}


// collision shoud not be available with sf::point primitive type (because of lack hitboxes)
void ParticleManage::particlePush(const vector<std::tuple<size_t, size_t, size_t>>& pushedParticlesIndex, bool collision)
{
	if (collision == true)
	{
		auto maxVelocity = 0.0f;
		// update of already existing particle groups
		for (auto pushedParticleIndex : pushedParticlesIndex)
		{
			//pushedParticlesIndex[0]
			auto [particleGroup, particleInGroup, forceWave] = pushedParticleIndex;

			auto actualParticle = m_explodedParticles.at(particleGroup)->getParticleVertex()[particleInGroup];
			auto& actualParticleAttribute = m_explodedParticles.at(particleGroup)->getParticleAttributes().at(particleInGroup);
			auto actualPosition = actualParticle.position;

			auto actualRadius = m_force[forceWave].getRadius();
			auto actualVelocity = sf::Vector2f{ (actualParticleAttribute.getVelocity().x - (getForceVelocity()/actualRadius)) , (actualParticleAttribute.getVelocity().y - (getForceVelocity()/actualRadius)) };
			
			auto tempVelocity = sqrt(pow(actualVelocity.x, 2) + pow(actualVelocity.y, 2));
			if (tempVelocity > maxVelocity) maxVelocity = tempVelocity;

			auto newDirectionVector = sf::Vector2f{ (actualPosition.x - m_force[forceWave].getPosition().x), (actualPosition.y - m_force[forceWave].getPosition().y) };
			auto newDirectionMagnitude = abs(newDirectionVector.x) + abs(newDirectionVector.y);
			auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };

			m_explodedParticles.at(particleGroup)->setParticleAttributes(particleInGroup, actualPosition, actualVelocity, newDirection);
		}
	//	std::cout << maxVelocity << "\n";
	}
}

void ParticleManage::vacuum(sf::Vector2i mousePosition)
{
	for (size_t i = 0; i < m_explodedParticles.size(); i++)
	{
		auto actualParticleGroup = m_explodedParticles[i]->getParticleVertex();

		for (size_t j = 0; j < actualParticleGroup.size(); j++)
		{
			auto& actualParticleAttribute = m_explodedParticles.at(i)->getParticleAttributes().at(j);

			auto newDirectionVector = sf::Vector2f{ mousePosition.x - actualParticleGroup[j].position.x, mousePosition.y - actualParticleGroup[j].position.y };
			auto newDirectionMagnitude = abs(newDirectionVector.x) + abs(newDirectionVector.y);
			auto newDirectionNormalized = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };

			auto actualVelocity = actualParticleAttribute.getVelocity();
			auto actualAcceleration = actualParticleAttribute.getAcceleration();
			
			actualVelocity.x += actualAcceleration.x * newDirectionNormalized.x;
			actualVelocity.y += actualAcceleration.y * newDirectionNormalized.y;

			if (abs(actualVelocity.x) + abs(actualVelocity.y) >= 40.0f) actualVelocity = sf::Vector2f{ 40.0f * newDirectionNormalized.x, 40.0f * newDirectionNormalized.y };
	
			m_explodedParticles.at(i)->setParticleAttributes(j, actualParticleGroup[j].position, actualVelocity, newDirectionNormalized);

			// sort particles by vector length and stop setting new attributes when it is nearer than...
		}
	}
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
}

void ParticleManage::forceUpdate()
{
	if (m_GravityOn == true) applyGravityForce(sf::Vector2f{ 0.0f,0.02f });
	if (m_AirResistanceOn == true) applyAirResistance();
	if (m_FrictionOn == true) applyFriction();
}



void ParticleManage::applyGravityForce(sf::Vector2f force)
{
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::unique_ptr<Particles>& particles) {particles->applyGravityForce(force); });
}

void ParticleManage::applyAirResistance(float coefficent)
{
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::unique_ptr<Particles>& particles) {particles->applyAirResistance(coefficent); });
}

void ParticleManage::applyFriction(float mi)
{
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::unique_ptr<Particles>& particles) {particles->applyFriction(mi); });
}

float ParticleManage::getRandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	float temp = distribution(m_generator);

	return temp;
}

void ParticleManage::update(float dt)
{
	forceUpdate();
	// for moving
	for (auto& particleGroup : m_explodedParticles)
	{
		particleGroup->update(dt* 2.0f);
	}
	
	forceWaveExpand(getForceVelocity() * 2.0f * dt, sf::Vector2f{ 1000.0f,1000.0f });
	

	auto particlesPushed = isForceWaveCollided();
	particlePush(particlesPushed.second, particlesPushed.first);
}

void ParticleManage::draw(sf::RenderWindow& window)
{
	for (const auto& particleGroup : m_explodedParticles)
	{
		particleGroup->draw(window);
	}
	for (const auto& force : m_force)
	{
		window.draw(force);
	}
}

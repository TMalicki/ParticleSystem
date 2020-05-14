#include "ParticleManage.h"
#include <algorithm>

using std::vector;

void ParticleManage::createParticles(sf::Vector2i mousePosition, int amount)
{
	// here reserve should be added to optimize that section
	if (m_type == ParticleType::Vertex)
	{
		m_explodedParticles.push_back(std::unique_ptr<ParticlesInterface>(new ParticlesVertex(amount, sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))));
	}
	else if (m_type == ParticleType::CircleShape)
	{
		m_explodedParticles.push_back(std::unique_ptr<ParticlesInterface>(new ParticlesCircle(amount, sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))));
	}
	//std::sort(m_explodedParticles.begin(),m_explodedParticles.end(),)
}

void ParticleManage::explode(sf::Vector2i mousePosition, sf::Vector2f randomRange, int amount)
{
	if (m_activeArea.x > mousePosition.x)
	{
		createParticles(mousePosition, amount);

		vector<sf::Vector2f> directionVector(amount);

		for (size_t index = 0; index < amount; index++)
		{
			float i = getRandomFloat(0.0f,2.0f*3.14f);
			directionVector[index] = sf::Vector2f(static_cast<float>(cos(i)), static_cast<float>(sin(i))); // this should be in as argument
		}
		setParticleExpandAttributes(m_explodedParticles, mousePosition, directionVector, randomRange);

		createForceWave(mousePosition);
	}
}

void ParticleManage::emitter(sf::Vector2i mousePosition, sf::Vector2f randomRange, int amount)
{
	if (m_activeArea.x > mousePosition.x)
	{

		createParticles(mousePosition, amount);

		vector<sf::Vector2f> directionVector(amount);

		for (size_t index = 0; index < amount; index++)
		{
			float i = getRandomFloat(230.0f * 3.14f / 180.0f, 310.0f * 3.14f / 180.0f);
			directionVector[index] = sf::Vector2f(static_cast<float>(cos(i)), static_cast<float>(sin(i))); // this should be in as argument
		}
		setParticleExpandAttributes(m_explodedParticles, mousePosition, directionVector, randomRange);
	}
}

void ParticleManage::setParticleExpandAttributes(vector<std::unique_ptr<ParticlesInterface>>& particleGroup, sf::Vector2i mousePosition, vector<sf::Vector2f> direction, sf::Vector2f randomRange)
{
	auto& actualParticleGroup = particleGroup.back();
	auto& actualParticleGroupAttributes = actualParticleGroup->getParticleAttributes();

	////////////////////////////////////////////////	////////////////////////////////////////////////	////////////////////////////////////////////////
	//ParticlesInterface* ptr = new ParticlesVertex(100);
	//auto* ptr2 = dynamic_cast<ParticlesVertex*>(ptr);
	//ptr2->getParticleVertex();
	////////////////////////////////////////////////	////////////////////////////////////////////////	

	// random forces, and random masses
	//vector<sf::Vector2f> randomDirections(actualParticleGroupAttributes.size());
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
	
	for (size_t k = 0; k < m_force.size(); k++)
	{
		auto actualForcePosition = m_force[k].getPosition();
		auto actualForceRadius = m_force[k].getRadius();

		for (size_t i = 0; i < m_explodedParticles.size(); i++)
		{
			 if(!((i == m_explodedParticles.size() - 1) && (k == m_force.size() - 1)))
			{
				//auto actualParticleGroup = m_explodedParticles[i]->getParticleVertex();
				auto size = m_explodedParticles[i]->getParticlesAmount();
				auto positionsVector = m_explodedParticles.at(i)->getPosition(); 

				for (size_t j = 0; j < size; j++)
				{
					auto positionFromForceWave = positionsVector.at(j) - actualForcePosition;
					auto distanceFromForceWave = sqrt(pow(positionFromForceWave.x, 2) + pow(positionFromForceWave.y, 2));

					if ((distanceFromForceWave - actualForceRadius > -40.0f) && (distanceFromForceWave - actualForceRadius) <= 20.0f)
					{
						indexes.push_back({ i,j, k });
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
	else if (force == ParticleSettings::Forces::External)
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
		applyWindForce(sf::Vector2f{ 0.02f,0.02f });
	}
}

void ParticleManage::applyWindForce(sf::Vector2f force)
{
	//std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::unique_ptr<ParticlesInterface>& particles) {particles->setDirection(sf::Vector2f{ 1.0f,0.0f }); });
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::unique_ptr<ParticlesInterface>& particles) {particles->applyForce(force); });
}

void ParticleManage::applyGravityForce(sf::Vector2f force)
{
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::unique_ptr<ParticlesInterface>& particles) {particles->applyGravityForce(force); });
}

void ParticleManage::applyAirResistance(float coefficent)
{
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::unique_ptr<ParticlesInterface>& particles) {particles->applyAirResistance(coefficent); });
}

void ParticleManage::applyFriction(float mi)
{
	std::for_each(m_explodedParticles.begin(), m_explodedParticles.end(), [&](std::unique_ptr<ParticlesInterface>& particles) {particles->applyFriction(mi); });
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
	forceWaveExpand(getWaveForce() * dt, m_activeArea);

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

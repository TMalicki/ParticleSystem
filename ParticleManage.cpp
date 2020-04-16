#include "ParticleManage.h"

Particles ParticleManage::createParticles(sf::PrimitiveType type, int amount)
{
	return Particles(amount, sf::Vector2f(0.0, 0.0), sf::Vector2f(2.0, 2.0), sf::Vector2f(0.0, 0.0), type);
}

void ParticleManage::explode(sf::Vector2i mousePosition, sf::PrimitiveType type, sf::Vector2f randomRange, int amount)
{
	m_explodedParticles.push_back(createParticles(type, amount));
	setParticleExpandAttributes(m_explodedParticles, mousePosition, type, randomRange, amount);

	createForceWave(mousePosition);
}

void ParticleManage::setParticleExpandAttributes(vector<Particles>& particleGroup, sf::Vector2i mousePosition, sf::PrimitiveType type, sf::Vector2f randomRange, int amount)
{
	auto& actualParticleGroup = particleGroup.back();
	auto actualParticleGroupAttributes = actualParticleGroup.getParticleAttributes();

	for (size_t i = 0; i < actualParticleGroup.getParticlesAmount(); i++)
	{
		float random_x = getRandomFloat(randomRange.x, randomRange.y);
		float random_y = getRandomFloat(randomRange.x, randomRange.y);

		auto tempVelocity = actualParticleGroupAttributes.at(i).getVelocity();
		tempVelocity += sf::Vector2f(random_x, random_y);

		sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
		actualParticleGroup.setParticleAttributes(i, mousePos, tempVelocity, sf::Vector2f(static_cast<float>(cos(i)), static_cast<float>(sin(i))));
	}
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
			//if (i != k) // to nie powinno byc to tylko nie powinien byc end() rowny end()
			{
				auto actualParticleGroup = m_explodedParticles[i].getParticleVertex();

				for (size_t j = 0; j < actualParticleGroup.getVertexCount(); j++)
				{
					auto positionFromForceWave = (actualParticleGroup[j].position - actualForcePosition);
					auto distanceFromForceWave = sqrt(pow(positionFromForceWave.x, 2) + pow(positionFromForceWave.y, 2));

					if ((distanceFromForceWave - actualForceRadius > -40.0f) && (distanceFromForceWave - actualForceRadius) <= 0.0f)
					{
						indexes.push_back({ i,j, k });
						m_explodedParticles[i].setParticleColor(j, sf::Color::Green);
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
		// update of already existing particle groups
		for (auto pushedParticleIndex : pushedParticlesIndex)
		{
			//pushedParticlesIndex[0]
			auto [particleGroup, particleInGroup, forceWave] = pushedParticleIndex;

			auto actualParticle = m_explodedParticles.at(particleGroup).getParticleVertex()[particleInGroup];
			auto actualParticleAttribute = m_explodedParticles.at(particleGroup).getParticleAttributes().at(particleInGroup);
			auto actualPosition = actualParticle.position;

			auto actualRadius = m_force[forceWave].getRadius();
			auto actualVelocity = sf::Vector2f{ (actualParticleAttribute.getVelocity().x - (getForceVelocity()/actualRadius)) , (actualParticleAttribute.getVelocity().y - (getForceVelocity()/actualRadius)) };

			auto newDirectionVector = sf::Vector2f{ (actualPosition.x - m_force[forceWave].getPosition().x), (actualPosition.y - m_force[forceWave].getPosition().y) };
			auto newDirectionMagnitude = abs(newDirectionVector.x + newDirectionVector.y);
			auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };

			m_explodedParticles.at(particleGroup).setParticleAttributes(particleInGroup, actualPosition, actualVelocity, newDirection);
		}
	}
}

void ParticleManage::vacuum()
{
	for (size_t i = 0; i < m_explodedParticles.size(); i++)
	{
		auto particles = m_explodedParticles[i].getParticleVertex();
		auto attributes = m_explodedParticles[i].getParticleAttributes();

		for (size_t j = 0; j < m_explodedParticles[i].getParticlesAmount(); j++)
		{

		}
	}
}

float ParticleManage::getRandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	float temp = distribution(m_generator);

	return temp;
}

void ParticleManage::update(float dt, sf::Vector2f windowSize)
{
	for (auto& particleGroup : m_explodedParticles)
	{
		particleGroup.update(dt);
	}
	
	forceWaveExpand(getForceVelocity()*dt, windowSize);
	auto particlesPushed = isForceWaveCollided();
	
	particlePush(particlesPushed.second, particlesPushed.first);
}

void ParticleManage::draw(sf::RenderWindow& window)
{
	for (const auto& particleGroup : m_explodedParticles)
	{
		window.draw(particleGroup.getParticleVertex());
	}
	for (const auto& force : m_force)
	{
		window.draw(force);
	}
}

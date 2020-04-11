#include "ParticleManage.h"

Particles ParticleManage::createParticles(sf::PrimitiveType type, int amount)
{
	return Particles(amount, sf::Vector2f(0.0, 0.0), sf::Vector2f(2.0, 2.0), sf::Vector2f(0.0, 0.0), type);
}

void ParticleManage::explode(sf::Vector2i mousePosition, sf::PrimitiveType type, sf::Vector2f randomRange, int amount)
{
	/// add breath on other groups nearer point of mouse == stronger
		//m_explodedParticles.push_back(Particles(amount, sf::Vector2f(0.0, 0.0), sf::Vector2f(2.0, 2.0),sf::Vector2f(0.0,0.0), type));

	m_explodedParticles.push_back(createParticles(type, amount));
	setParticleExpandAttributes(m_explodedParticles, mousePosition, type, randomRange, amount);

	createForceWave(mousePosition);

	//m_force.push_back(createParticles(sf::TriangleFan, amount));
	//particleExpand(m_force, mousePosition, sf::TriangleFan, sf::Vector2f(0.0,0.0), amount);
	//ForceExpand(mousePosition, sf::TriangleFan);
	//particlePush();
}

void ParticleManage::setParticleExpandAttributes(vector<Particles>& particleGroup, sf::Vector2i mousePosition, sf::PrimitiveType type, sf::Vector2f randomRange, int amount)
{
	//std::default_random_engine generator;
	//std::uniform_real_distribution<float> distribution(1.0, 6.0);

	auto& actualParticleGroup = particleGroup.back();
	auto actualParticleGroupAttributes = actualParticleGroup.getParticleAttributes();

	for (size_t i = 0; i < actualParticleGroup.getParticlesAmount(); i++)
	{
		float random_x = getRandomFloat(randomRange.x, randomRange.y);
		float random_y = getRandomFloat(randomRange.x, randomRange.y);

		auto tempVelocity = actualParticleGroupAttributes.at(i).getVelocity();
		tempVelocity += sf::Vector2f(random_x, random_y);

		sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
		actualParticleGroup.setParticleAttributes(i, mousePos, tempVelocity, sf::Vector2f(static_cast<float>(sin(i)), static_cast<float>(cos(i))));
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
}

void ParticleManage::forceWaveExpand(float velocity)
{
	for (auto& force : m_force)
	{
		force.setRadius(force.getRadius() + velocity);
		force.setOrigin(force.getRadius(), force.getRadius());
	}
}

const auto ParticleManage::isForceWaveCollided()
{

	// nie powinny byc liczone fale uderzeniowe ktore wyszly juz poza ekran
	// czasteczki ktore wychodza poza ekran powinny byc w zaleznosci od trybu usuwane, odbijane od krawedzi, lub powinny wychodzic z innej strony ekranu 

	bool collision = false;
	vector<std::pair<size_t, size_t>> indexes{};
	
	for (size_t k = 0; k < m_force.size(); k++)
	{
		auto actualForcePosition = m_force[k].getPosition();
		auto actualForceRadius = m_force[k].getRadius();

		for (size_t i = 0; i < m_explodedParticles.size(); i++)
		{
			if (i != k)
			{
				auto actualParticleGroup = m_explodedParticles[i].getParticle();

				for (size_t j = 0; j < actualParticleGroup.getVertexCount(); j++)
				{
					auto positionFromForceWave = (actualParticleGroup[j].position - actualForcePosition);
					auto distanceFromForceWave = sqrt(pow(positionFromForceWave.x, 2) + pow(positionFromForceWave.y, 2));

					if ((distanceFromForceWave - actualForceRadius > -50.0f) && (distanceFromForceWave - actualForceRadius) <= 0.0f)
					{
						indexes.push_back({ i,j });
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
void ParticleManage::particlePush(const vector<std::pair<size_t, size_t>>& pushedParticlesIndex, bool collision)
{
	if (collision == true)
	{
		//auto forceSource = m_force.back();

		// update of already existing particle groups
		for (size_t i = 0; i < pushedParticlesIndex.size(); i++)
		{
			auto actualParticle = m_explodedParticles.at(pushedParticlesIndex[i].first).getParticle()[pushedParticlesIndex[i].second];
			auto actualParticleAttribute = m_explodedParticles.at(pushedParticlesIndex[i].first).getParticleAttributes().at(pushedParticlesIndex[i].second);


			/*
				sf::Vector2f actualVelocity = sf::Vector2f(actualParticleAttribute.getVelocity().x - (forceSource.at(j).getVelocity().x - 0.5f),
					actualParticleAttribute.getVelocity().y - (forceSource.at(j).getVelocity().y - 0.5f));
				sf::Vector2f actualDirection = sf::Vector2f(actualParticleAttribute.getDirection().x - (forceSource.at(j).getDirection().x - 0.5f),
					actualParticleAttribute.getDirection().y - (forceSource.at(j).getDirection().y - 0.5f));
			*/
			auto actualPosition = m_explodedParticles.at(pushedParticlesIndex[i].first).getParticle()[pushedParticlesIndex[i].second].position;
			auto actualVelocity = actualParticleAttribute.getVelocity();
			auto actualDirection = -actualParticleAttribute.getDirection();

			m_explodedParticles.at(pushedParticlesIndex[i].first).setParticleAttributes(pushedParticlesIndex[i].second, actualPosition, actualVelocity, actualDirection);
		}
	}
}

void ParticleManage::vacuum()
{
	for (size_t i = 0; i < m_explodedParticles.size(); i++)
	{
		auto particles = m_explodedParticles[i].getParticle();
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

void ParticleManage::update(float dt)
{
	for (auto& particleGroup : m_explodedParticles)
	{
		particleGroup.update(dt);
	}
	
	forceWaveExpand(0.2f*dt);

	auto particlesPushed = isForceWaveCollided();
	particlePush(particlesPushed.second, particlesPushed.first);
}

void ParticleManage::draw(sf::RenderWindow& window)
{
	for (const auto& particleGroup : m_explodedParticles)
	{
		window.draw(particleGroup.getParticle());
	}
	for (const auto& force : m_force)
	{
		window.draw(force);
	}
}

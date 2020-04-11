#include "Particles.h"

Particles::Particles(long int amount, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction, sf::PrimitiveType tempType) :
	m_type(tempType), m_particles(tempType, amount), m_particleAttributes(amount)
{
	for (size_t i = 0; i < m_particles.getVertexCount(); i++)
	{
		setParticleAttributes(i, position, velocity, direction);
	}
}

void Particles::update(float dt) // in parameter - dt
{
	for (size_t i = 0; i < m_particles.getVertexCount(); i++)
	{
		sf::Vector2f tempDirection = m_particleAttributes[i].getDirection();
		sf::Vector2f tempVelocity = m_particleAttributes[i].getVelocity();
		m_particles[i].position.x += tempDirection.x * tempVelocity.x * dt / 100.0f;	//s = s0 + v*t
		m_particles[i].position.y += tempDirection.y * tempVelocity.y * dt / 100.0f;
	}
}

void Particles::setParticleAttributes(size_t index, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction)
{
//	for (size_t i = 0; i < m_particles.getVertexCount(); i++)
//	{
		m_particles[index].position = position;
		m_particleAttributes[index].setVelocity(velocity);
		m_particleAttributes[index].setDirection(direction);
//	}
}

/*
void Particles::move(sf::Vector2i mousePosition)
{
	int randomPositionX;
	int randomPositionY;
	sf::Vector2f actualPosition;
	int valueOfColor;

	for (size_t i = 0; i < m_particles.getVertexCount(); i++)
	{
		actualPosition = m_particles[i].position;
		randomPositionX = (rand() % 100 - 50) / 2;
		randomPositionY = (rand() % 100 - 50) / 2;
		sf::Vector2f diffPosition((mousePosition.x - actualPosition.x) / 50 + randomPositionX, (mousePosition.y - actualPosition.y) / 50 + randomPositionY);
		m_particles[i].position += diffPosition;

		//particles[i].position += sf::Vector2f(randomPositionX, randomPositionY);

		valueOfColor = static_cast<int>((sqrt(pow(mousePosition.x - actualPosition.x, 2) + pow(mousePosition.y - actualPosition.y, 2))) * 255.0 / 1900.0);
		//std::cout << valueOfColor << std::endl;

		m_particles[i].color = sf::Color(0, 255 - valueOfColor, 0, 255);
	}
}
*/
/*
ParticleGroup::ParticleGroup(int amount) : particleGroup(amount), vertices(type, amount)
{
	srand(time(NULL));


	std::cout << "Utworzono particleGroup\n";
}
*/
/*
void Particles::explode(sf::Vector2i mousePosition)
{
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(1.0, 6.0);
	
	for (size_t i = 0; i < m_particles.getVertexCount(); i++)
	{
		auto random_x = distribution(generator);
		auto random_y = distribution(generator);
		auto tempVelocity = m_particleAttributes[i].getVelocity();
		tempVelocity += sf::Vector2f(random_x, random_y);

		sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
		setParticleAttributes(i, mousePos, tempVelocity, sf::Vector2f(static_cast<float>(sin(i)), static_cast<float>(cos(i))));	
	}
}
*/




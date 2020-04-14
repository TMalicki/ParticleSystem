#include "Particles.h"

Particles::Particles(long int amount, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction, sf::PrimitiveType tempType) :
	m_type(tempType), m_particlesVertex(tempType, amount), m_particleAttributes(amount)
{
	for (size_t i = 0; i < m_particlesVertex.getVertexCount(); i++)
	{
		setParticleAttributes(i, position, velocity, direction);
	}
}

void Particles::update(float dt) // in parameter - dt
{
	for (size_t i = 0; i < m_particlesVertex.getVertexCount(); i++)
	{
		sf::Vector2f tempDirection = m_particleAttributes[i].getDirection();
		sf::Vector2f tempVelocity = m_particleAttributes[i].getVelocity();
		m_particlesVertex[i].position.x += tempDirection.x * tempVelocity.x * dt / 100.0f;	//s = s0 + v*t
		m_particlesVertex[i].position.y += tempDirection.y * tempVelocity.y * dt / 100.0f;
	}
}

void Particles::setParticleAttributes(size_t index, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction)
{
	m_particlesVertex[index].position = position;
	m_particleAttributes[index].setVelocity(velocity);
	m_particleAttributes[index].setDirection(direction);
}

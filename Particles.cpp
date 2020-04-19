#include "Particles.h"

sf::Vector2f Particles::s_activeAreaSize = sf::Vector2f(0.0f, 0.0f);

Particles::Particles(long int amount, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction, sf::PrimitiveType tempType) :
	m_type(tempType), m_particlesVertex(tempType, amount), m_particleAttributes(amount)
{
	for (size_t i = 0; i < m_particlesVertex.getVertexCount(); i++)
	{
		setParticleAttributes(i, position, velocity, direction);
	}
//	auto lambdaFun = [this, position, velocity, direction](size_t index) mutable { setParticleAttributes(index, position, velocity, direction); };
//	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), lambdaFun);
}

void Particles::update(float dt) // in parameter - dt
{
	for (size_t i = 0; i < m_particlesVertex.getVertexCount(); i++)
	{
		auto tempPosition = m_particlesVertex[i].position;
		if (tempPosition.x >= 0 && tempPosition.x <= s_activeAreaSize.x && tempPosition.y >= 0 && tempPosition.y <= s_activeAreaSize.y)
		{
			sf::Vector2f tempDirection = m_particleAttributes[i].getDirection();
			sf::Vector2f tempVelocity = m_particleAttributes[i].getVelocity();
			m_particlesVertex[i].position.x += tempDirection.x * tempVelocity.x * dt / 100.0f;	//s = s0 + v*t
			m_particlesVertex[i].position.y += tempDirection.y * tempVelocity.y * dt / 100.0f;
		}
		else
		{
			if (tempPosition.y < 0) m_particlesVertex[i].position.y = s_activeAreaSize.y;
			else if (tempPosition.y > s_activeAreaSize.y) m_particlesVertex[i].position.y = 0;
			else if (tempPosition.x < 0) m_particlesVertex[i].position.x = s_activeAreaSize.x;
			else if (tempPosition.x > s_activeAreaSize.x) m_particlesVertex[i].position.x = 0;
		}
	}
}

void Particles::setParticleAttributes(size_t index, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction)
{
	m_particlesVertex[index].position = position;
	m_particleAttributes[index].setVelocity(velocity);
	m_particleAttributes[index].setDirection(direction);
}

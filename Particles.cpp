#include "Particles.h"

sf::Vector2f Particles::s_activeAreaSize = sf::Vector2f(1920.0f, 1080.0f);

Particles::Particles(long int amount, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction, sf::PrimitiveType tempType) :
	m_type(tempType), m_particleVertex(tempType, amount), m_particleAttributes(amount)
{
	for (size_t i = 0; i < m_particleVertex.getVertexCount(); i++)
	{
		m_particleVertex[i].position = position;
		m_particleAttributes[i].setVelocity(velocity);
		m_particleAttributes[i].setDirection(direction);
		//setParticleAttributes(i, position, velocity, direction);
		m_particleAttributes[i].setAcceleration(sf::Vector2f{ 0.1f, 0.1f });
	}
//	auto lambdaFun = [this, position, velocity, direction](size_t index) mutable { setParticleAttributes(index, position, velocity, direction); };
//	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), lambdaFun);
}

void Particles::update(float dt) // in parameter - dt
{
	for (size_t i = 0; i < m_particleVertex.getVertexCount(); i++)
	{
		auto tempPosition = m_particleVertex[i].position;
		if (tempPosition.x >= 0 && tempPosition.x <= s_activeAreaSize.x && tempPosition.y >= 0 && tempPosition.y <= s_activeAreaSize.y)
		{
			sf::Vector2f tempDirection = m_particleAttributes[i].getDirection();
			sf::Vector2f tempVelocity = m_particleAttributes[i].getVelocity();
			sf::Vector2f tempAcceleration = m_particleAttributes[i].getAcceleration();

			tempVelocity.x += tempAcceleration.x * tempDirection.x;
			tempVelocity.y += tempAcceleration.y * tempDirection.y;

			float magnitudeVector = sqrt(pow(tempVelocity.x, 2) + pow(tempVelocity.y, 2));
			if (magnitudeVector > 100.0f)
			{
				tempVelocity = tempVelocity / magnitudeVector * 100.0f;
			}

			tempPosition += tempVelocity * dt / 100.0f;

			setParticleAttributes(i, tempPosition, tempVelocity, tempDirection);
		}
		else
		{
			if (tempPosition.y < 0) m_particleVertex[i].position.y = s_activeAreaSize.y;
			else if (tempPosition.y > s_activeAreaSize.y) m_particleVertex[i].position.y = 0;
			else if (tempPosition.x < 0) m_particleVertex[i].position.x = s_activeAreaSize.x;
			else if (tempPosition.x > s_activeAreaSize.x) m_particleVertex[i].position.x = 0;
		}
	}
}

void Particles::moveTowardsPoint(sf::Vector2f goalPosition)
{
	for (int i = 0; i < m_particleVertex.getVertexCount(); i++)
	{
		auto actualPosition = getParticleVertex()[i].position;
		auto newDirectionVector = sf::Vector2f{ goalPosition.x - actualPosition.x, goalPosition.y - actualPosition.y };
		auto newDirectionMagnitude = sqrt(pow(newDirectionVector.x, 2) + pow(newDirectionVector.y, 2));
		if (newDirectionMagnitude == 0.0f) newDirectionMagnitude = 1.0f;
		auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };

		setParticleAttributes(i, actualPosition, getParticleAttributes()[i].getVelocity(), newDirection);
	}
}

void Particles::setParticleAttributes(size_t index, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction)
{
	m_particleVertex[index].position = position;
	m_particleAttributes[index].setVelocity(velocity);
	m_particleAttributes[index].setDirection(direction);
}

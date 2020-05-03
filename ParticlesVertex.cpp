#include "ParticlesVertex.h"
#include <algorithm>

ParticlesVertex::ParticlesVertex(long int amount = 100, sf::Vector2f position = { 0.0,0.0 }) : m_particleVertex(amount), ParticlesInterface(amount, position)
{
	calculateVerticesPosition();
}

void ParticlesVertex::calculateVerticesPosition()
{
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		m_particleVertex[i].position = m_particleAttributes[i].getPosition();
	}
}

void ParticlesVertex::setDirectionTowardsPoint(sf::Vector2f goalPosition)
{
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		auto actualPosition = m_particleVertex[i].position;
		auto newDirectionVector = sf::Vector2f{ goalPosition.x - actualPosition.x, goalPosition.y - actualPosition.y };
		auto newDirectionMagnitude = sqrt(pow(newDirectionVector.x, 2) + pow(newDirectionVector.y, 2));
		if (newDirectionMagnitude == 0.0f) newDirectionMagnitude = 1.0f;

		auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };
		m_particleAttributes[i].setDirection(newDirection);
	}
}

void ParticlesVertex::update(float dt) // in parameter - dt
{
	//forceUpdate();
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		// is it needed to update both: position of each pixel and object of pixels like sf::Lines?
		auto tempPosition = m_particleVertex[i].position;
		auto maxVelocity = getMaxVelocity();

		//if (tempPosition.x >= 0 && tempPosition.x <= s_activeAreaSize.x && tempPosition.y >= 0 && tempPosition.y <= s_activeAreaSize.y)
		//{
			sf::Vector2f tempDirection = m_particleAttributes[i].getDirection();
			sf::Vector2f tempVelocity = m_particleAttributes[i].getVelocity();
			sf::Vector2f tempAcceleration = m_particleAttributes[i].getAcceleration();

			tempVelocity.x += tempAcceleration.x; 
			tempVelocity.y += tempAcceleration.y; 

			float magnitudeVector = sqrt(pow(tempVelocity.x, 2) + pow(tempVelocity.y, 2));
			if (magnitudeVector > maxVelocity)
			{
				tempVelocity = tempVelocity / magnitudeVector * maxVelocity;
			}

			tempPosition += tempVelocity * dt / 100.0f;

			setParticleAttributesN(i, tempPosition, tempVelocity);
			//setParticleAttributes(i, tempPosition, tempVelocity, tempDirection);

			m_particleAttributes[i].setAcceleration(sf::Vector2f{ 0.0f, 0.0f });
	}
}


void ParticlesVertex::setParticleAttributesN(size_t index, sf::Vector2f position, sf::Vector2f velocity)
{
	m_particleVertex[index].position = position;
	m_particleAttributes[index].setVelocity(velocity);
}

#include "Particles.h"
#include <algorithm>

Particles::Particles(long int amount, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction, sf::PrimitiveType tempType) :
	m_type(tempType), m_particleVertex(amount), m_particleAttributes(amount)
{
	
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		m_particleVertex[i].position = position;
		m_particleAttributes[i].setVelocity(velocity);
		m_particleAttributes[i].setDirection(direction);
		//setParticleAttributes(i, position, velocity, direction);
		////m_particleAttributes[i].setAcceleration(sf::Vector2f{ 0.1f, 0.1f });
		////m_particleAttributes[i].applyForce(sf::Vector2f(0.0f, 1.0f));
	}
	
	//auto lambdaFun = [this, position, velocity, direction](size_t index) mutable { setParticleAttributes(index, position, velocity, direction); };
	//std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), lambdaFun);
}

void Particles::update(float dt) // in parameter - dt
{
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		auto tempPosition = m_particleVertex[i].position;
		//if (tempPosition.x >= 0 && tempPosition.x <= s_activeAreaSize.x && tempPosition.y >= 0 && tempPosition.y <= s_activeAreaSize.y)
		//{
			sf::Vector2f tempDirection = m_particleAttributes[i].getDirection();
			sf::Vector2f tempVelocity = m_particleAttributes[i].getVelocity();
			sf::Vector2f tempAcceleration = m_particleAttributes[i].getAcceleration();

			tempVelocity.x += tempAcceleration.x; 
			tempVelocity.y += tempAcceleration.y; 

			float magnitudeVector = sqrt(pow(tempVelocity.x, 2) + pow(tempVelocity.y, 2));
			if (magnitudeVector > 100.0f)
			{
				tempVelocity = tempVelocity / magnitudeVector * 100.0f;
			}

			tempPosition += tempVelocity * dt / 100.0f;

			setParticleAttributes(i, tempPosition, tempVelocity, tempDirection);

			m_particleAttributes[i].setAcceleration(sf::Vector2f{ 0.0f, 0.0f });
	}
}

void Particles::getDirectionTowardsPoint(sf::Vector2f goalPosition)
{
	/*
	std::vector<vector2f> temp;
	//for(const auto& particle : m_particleVertex)
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particleAttribute, sf::Vertex& particleVertex) mutable {
		auto actualPosition = particleVertex.position;
		auto newDirectionVector = sf::Vector2f{ goalPosition.x - actualPosition.x, goalPosition.y - actualPosition.y };
		auto newDirectionMagnitude = sqrt(pow(newDirectionVector.x, 2) + pow(newDirectionVector.y, 2));
		if (newDirectionMagnitude == 0.0f) newDirectionMagnitude = 1.0f;
		auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };

		particleAttribute.setDirection(newDirection);
		});

	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particleAttribute, sf::Vertex& particleVertex) mutable {
	*/
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		auto actualPosition = m_particleVertex[i].position;
		auto newDirectionVector = sf::Vector2f{ goalPosition.x - actualPosition.x, goalPosition.y - actualPosition.y };
		auto newDirectionMagnitude = sqrt(pow(newDirectionVector.x, 2) + pow(newDirectionVector.y, 2));
		if (newDirectionMagnitude == 0.0f) newDirectionMagnitude = 1.0f;
		auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };

		m_particleAttributes[i].setDirection(newDirection);
		//setParticleAttributes(i, actualPosition, getParticleAttributes()[i].getVelocity(), newDirection);
	}
	
}

void Particles::applyForce(sf::Vector2f force, ParticleSettings::Forces forceType, float constant)
{
	
	for (auto& particle : m_particleAttributes)
	{
		particle.applyForce(force, forceType, constant);
	}
}

void Particles::applyGravityForce(sf::Vector2f force)
{
	applyForce(force, ParticleSettings::Forces::Gravity);
}

void Particles::applyAirResistance(float coefficent)
{
	applyForce(sf::Vector2f{ 0.0f, 0.0f }, ParticleSettings::Forces::AirResistance, coefficent);
}

void Particles::applyFriction(float mi)
{
	applyForce(sf::Vector2f{ 0.0f, 0.0f }, ParticleSettings::Forces::Friction, mi);
}

void Particles::setParticleAttributes(size_t index, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction)
{
	m_particleVertex[index].position = position;
	m_particleAttributes[index].setVelocity(velocity);
	m_particleAttributes[index].setDirection(direction);
}

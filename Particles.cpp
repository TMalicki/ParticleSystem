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
	}
}

void Particles::update(float dt) // in parameter - dt
{
	forceUpdate();
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

			setParticleAttributesN(i, tempPosition, tempVelocity);
			//setParticleAttributes(i, tempPosition, tempVelocity, tempDirection);

			m_particleAttributes[i].setAcceleration(sf::Vector2f{ 0.0f, 0.0f });
	}
}

void Particles::getDirectionTowardsPoint(sf::Vector2f goalPosition)
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

void Particles::TurnOnForce(bool logic, ParticleSettings::Forces force)
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

void Particles::forceUpdate()
{
	if (m_GravityOn == true) applyGravityForce(sf::Vector2f{ 0.0f,0.02f });
	if (m_AirResistanceOn == true) applyAirResistance();
	if (m_FrictionOn == true) applyFriction();
}

void Particles::applyForce(sf::Vector2f force, ParticleSettings::Forces forceType, float constant)
{
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [=](ParticleSettings& particle) {particle.applyForce(force, forceType, constant); });
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

void Particles::setParticleAttributesN(size_t index, sf::Vector2f position, sf::Vector2f velocity)
{
	m_particleVertex[index].position = position;
	m_particleAttributes[index].setVelocity(velocity);
}

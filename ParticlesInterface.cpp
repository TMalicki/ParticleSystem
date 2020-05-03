#include "ParticlesVertex.h"
#include <algorithm>

ParticlesInterface::ParticlesInterface(long int amount, sf::Vector2f position) : m_particleAttributes(amount), m_maxVelocity(100.0f)
{
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particle) {particle.setPosition(position); });
}

void ParticlesInterface::setDirection(size_t index, sf::Vector2f direction)
{
	m_particleAttributes[index].setDirection(direction);
}

void ParticlesInterface::setDirection(std::vector<sf::Vector2f> directionVector)
{
	for (size_t i = 0; i < directionVector.size(); i++)
	{
		m_particleAttributes[i].setDirection(directionVector.at(i));
	}
}
void ParticlesInterface::applyForce(size_t index, sf::Vector2f force, ParticleSettings::Forces forceType, float constant)
{
	m_particleAttributes[index].applyForce(force, forceType, constant);
}

void ParticlesInterface::applyForce(sf::Vector2f force, ParticleSettings::Forces forceType, float constant)
{
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [=](ParticleSettings& particle) {particle.applyForce(force, forceType, constant); });
}

void ParticlesInterface::applyForce(std::vector<sf::Vector2f> forceVector, ParticleSettings::Forces forceType, float constant)
{
	for (size_t i = 0; i < forceVector.size(); i++)
	{
		m_particleAttributes[i].applyForce(forceVector.at(i), forceType, constant);
	}
}

void ParticlesInterface::applyGravityForce(sf::Vector2f force)
{
	applyForce(force, ParticleSettings::Forces::Gravity);
}

void ParticlesInterface::applyAirResistance(float coefficent)
{
	applyForce(sf::Vector2f{ 0.0f, 0.0f }, ParticleSettings::Forces::AirResistance, coefficent);
}

void ParticlesInterface::applyFriction(float mi)
{
	applyForce(sf::Vector2f{ 0.0f, 0.0f }, ParticleSettings::Forces::Friction, mi);
}

void ParticlesInterface::setMass(std::vector<float> masses)
{
	for (size_t i = 0; i < masses.size(); i++)
	{
		m_particleAttributes[i].setMass(masses[i]);
	}
}

/*
void ParticlesInterface::update(float dt) // in parameter - dt
{
	//forceUpdate();
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		// is it needed to update both: position of each pixel and object of pixels like sf::Lines?
		auto tempPosition = m_particleVertex[i].position;
		//if (tempPosition.x >= 0 && tempPosition.x <= s_activeAreaSize.x && tempPosition.y >= 0 && tempPosition.y <= s_activeAreaSize.y)
		//{
		sf::Vector2f tempDirection = m_particleAttributes[i].getDirection();
		sf::Vector2f tempVelocity = m_particleAttributes[i].getVelocity();
		sf::Vector2f tempAcceleration = m_particleAttributes[i].getAcceleration();

		tempVelocity.x += tempAcceleration.x;
		tempVelocity.y += tempAcceleration.y;

		float magnitudeVector = sqrt(pow(tempVelocity.x, 2) + pow(tempVelocity.y, 2));
		if (magnitudeVector > m_maxVelocity)
		{
			tempVelocity = tempVelocity / magnitudeVector * m_maxVelocity;
		}

		tempPosition += tempVelocity * dt / 100.0f;

		setParticleAttributesN(i, tempPosition, tempVelocity);
		//setParticleAttributes(i, tempPosition, tempVelocity, tempDirection);

		m_particleAttributes[i].setAcceleration(sf::Vector2f{ 0.0f, 0.0f });
	}
}





void ParticlesInterface::setParticleAttributes(size_t index, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f direction)
{
	m_particleVertex[index].position = position;
	m_particleAttributes[index].setVelocity(velocity);
	m_particleAttributes[index].setDirection(direction);
}
*/
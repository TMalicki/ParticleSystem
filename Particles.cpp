#include "Particles.h"
#include <algorithm>

Particles::Particles(long int amount, sf::Vector2f position,  sf::PrimitiveType tempType) :
	m_type(tempType), m_particleVertex(amount), m_maxVelocity(30.0f)
{
	auto edgeAmount = getEdgeAmount(tempType);

	m_particleAttributes = std::move(std::vector<ParticleSettings>(amount / edgeAmount));
	std::for_each(m_particleVertex.begin(), m_particleVertex.end(), [&](sf::Vertex& particle) {particle.position = position; });
}

int Particles::getEdgeAmount(sf::PrimitiveType tempType)
{
	Types type{};

	if (tempType == sf::PrimitiveType::Points) type = Types::Points;
	else if (tempType == sf::PrimitiveType::Lines) type = Types::Lines;
	else if (tempType == sf::PrimitiveType::Triangles) type = Types::Triangles;
	else if (tempType == sf::PrimitiveType::Quads) type = Types::Quads;
	else if (tempType == sf::PrimitiveType::TriangleFan) type = Types::Circles;

	return static_cast<int>(type);
}

void Particles::update(float dt) // in parameter - dt
{
	//forceUpdate();
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

void Particles::setDirectionTowardsPoint(sf::Vector2f goalPosition)
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

void Particles::setDirection(size_t index, sf::Vector2f direction)
{
	m_particleAttributes[index].setDirection(direction);
}

void Particles::setDirection(std::vector<sf::Vector2f> directionVector)
{
	for (size_t i = 0; i < directionVector.size(); i++)
	{
		m_particleAttributes[i].setDirection(directionVector.at(i));
	}
}
void Particles::applyForce(size_t index, sf::Vector2f force, ParticleSettings::Forces forceType, float constant)
{
	m_particleAttributes[index].applyForce(force, forceType, constant);
}

void Particles::applyForce(sf::Vector2f force, ParticleSettings::Forces forceType, float constant)
{
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [=](ParticleSettings& particle) {particle.applyForce(force, forceType, constant); });
}

void Particles::applyForce(std::vector<sf::Vector2f> forceVector, ParticleSettings::Forces forceType, float constant)
{
	for (size_t i = 0; i < forceVector.size(); i++)
	{
		m_particleAttributes[i].applyForce(forceVector.at(i), forceType, constant);
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

void Particles::setMass(std::vector<float> masses)
{
	for (size_t i = 0; i < masses.size(); i++)
	{
		m_particleAttributes[i].setMass(masses[i]);
	}
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

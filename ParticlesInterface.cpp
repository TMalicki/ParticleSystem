#include "ParticlesInterface.h"
#include <algorithm>

float ParticlesInterface::m_maxVelocity = 100.0f;

ParticlesInterface::ParticlesInterface(long int amount, sf::Vector2f position) :
	m_particleAttributes(amount)//, m_lifeTimeMs(1000.0f)
{
	//std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particle) {particle.setPosition(position); });
}

ParticlesInterface::ParticlesInterface(long int amount, std::vector<sf::Vector2f> position) :
	m_particleAttributes(amount)
{

}

void ParticlesInterface::setDirection(sf::Vector2f direction)
{
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particleSettings) {particleSettings.setDirection(direction);});
}

void ParticlesInterface::setDirection(size_t index, sf::Vector2f direction)
{
	m_particleAttributes[index].setDirection(direction);
}

void ParticlesInterface::setDirection(std::vector<sf::Vector2f> directionVector)
{
	size_t index{};
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particleSettings) {particleSettings.setDirection(directionVector[index]); index++; });
}
void ParticlesInterface::applyForce(size_t index, sf::Vector2f force, ParticleSettings::Forces forceType, float constant)
{
	m_particleAttributes[index].applyForce(force, forceType, constant);
}

void ParticlesInterface::applyForce(sf::Vector2f force, ParticleSettings::Forces forceType, float constant)
{
	// the same force for every particle
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particle) {particle.applyForce(force, forceType, constant); });
}

void ParticlesInterface::applyForce(std::vector<sf::Vector2f> forceVector, ParticleSettings::Forces forceType, float constant)
{
	// different force for every particle
	size_t index{};
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particleSettings) {particleSettings.applyForce(forceVector[index], forceType, constant); index++; });
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
	size_t index{};
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particleSettings) {particleSettings.setMass(masses[index]); index++; });
}


const std::vector<sf::Vector2f> ParticlesInterface::getVelocity()
{
	std::vector<sf::Vector2f> tempVelocity(m_particleAttributes.size());
	size_t counter{};
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particleSetting) {tempVelocity[counter] = particleSetting.getVelocity(); counter++; });

	return tempVelocity;
}

void ParticlesInterface::setVelocity(std::vector<sf::Vector2f> velocities)
{
	size_t index{};
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particleSetting) {particleSetting.setVelocity(velocities[index]); index++; });
}

void ParticlesInterface::toErase()
{
	auto cutOff = std::lower_bound(m_particleAttributes.begin(), m_particleAttributes.end(), 0.0f, [&](ParticleSettings attributes, const float b) { return attributes.getLifeTime() < b; });
	eraseParticles(cutOff);
}

void ParticlesInterface::reduceLifeTime(float dt, float sum, float maxLifeTime)
{
	size_t index{};
	std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&](ParticleSettings& particleSetting) {particleSetting.reduceLifeTime(dt); index++; });

	if (sum >= (maxLifeTime / 255.0f))
	{
		reduceColorOpacity(1);
	}
	toErase();
}
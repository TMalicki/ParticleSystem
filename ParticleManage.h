#pragma once
#include <iostream>
#include <random>
#include "Particles.h"

class ParticleManage
{
private:
	std::default_random_engine					m_generator;
	sf::Vector2f								m_activeAreaSize;


	std::vector<std::unique_ptr<Particles>>			m_explodedParticles;
	std::vector<sf::CircleShape>						m_force;	// make class for m_force
	float										m_forceWaveVelocity;
public:
	ParticleManage() : m_forceWaveVelocity{ 0 }, m_activeAreaSize{ sf::Vector2f(0.0f, 0.0f) } {};
	//void setActiveAreaSize(sf::Vector2f activeAreaSize) { m_activeAreaSize = activeAreaSize; Particles::setActiveArea(activeAreaSize); }
	//const sf::Vector2f getActiveAreaSize() { return m_activeAreaSize; }

	void explode(sf::Vector2i, sf::PrimitiveType, sf::Vector2f randomRange = sf::Vector2f(0.0f, 0.0f), int amount = 1000);
	void vacuum(sf::Vector2i);

	void createParticles(sf::PrimitiveType type = sf::Points, int amount = 1000);
	void setParticleExpandAttributes(std::vector<std::unique_ptr<Particles>>&, sf::Vector2i, sf::PrimitiveType, sf::Vector2f randomRange = sf::Vector2f(0.0f, 0.0f), int amount = 1000);
	
	void particlePush(const std::vector<std::tuple<size_t, size_t, size_t>>&, bool collision = false);
	void createForceWave(sf::Vector2i, float radius = 1.0f);
	void forceWaveExpand(float, sf::Vector2f);
	const auto isForceWaveCollided();

	void setForceVelocity(float velocity) { m_forceWaveVelocity = velocity; }
	const float getForceVelocity() { return m_forceWaveVelocity; }

	float getRandomFloat(float, float);
	void update(float dt);
	void draw(sf::RenderWindow&);
};

// predefinedExplosion zamiast explosion
// predefinedVacuum

// effectForcePush zamiast force push?
// effectExplosionForce
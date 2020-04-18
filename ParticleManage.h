#pragma once
#include <iostream>
#include <random>
#include "Particles.h"

class ParticleManage
{
private:
	std::default_random_engine  m_generator;

	vector<std::unique_ptr<Particles>>			m_explodedParticles;
	vector<sf::CircleShape>		m_force;	// make class for m_force
	float						m_forceWaveVelocity;
public:
	ParticleManage() : m_forceWaveVelocity(0) {};
	
	void explode(sf::Vector2i, sf::PrimitiveType, sf::Vector2f randomRange = sf::Vector2f(0.0f, 0.0f), int amount = 1000);
	void vacuum(sf::Vector2i);

	void createParticles(sf::PrimitiveType type = sf::Points, int amount = 1000);
	void setParticleExpandAttributes(vector<std::unique_ptr<Particles>>&, sf::Vector2i, sf::PrimitiveType, sf::Vector2f randomRange = sf::Vector2f(0.0f, 0.0f), int amount = 1000);
	
	void particlePush(const vector<std::tuple<size_t, size_t, size_t>>&, bool collision = false);
	void createForceWave(sf::Vector2i, float radius = 1.0f);
	void forceWaveExpand(float, sf::Vector2f);
	const auto isForceWaveCollided();

	void setForceVelocity(float velocity) { m_forceWaveVelocity = velocity; }
	const float getForceVelocity() { return m_forceWaveVelocity; }

	float getRandomFloat(float, float);
	void update(float dt, sf::Vector2f);
	void draw(sf::RenderWindow&);
};

// predefinedExplosion zamiast explosion
// predefinedVacuum

// effectForcePush zamiast force push?
// effectExplosionForce
#pragma once
#include <iostream>
#include <random>
#include "Particles.h"

class ParticleManage
{
private:
	std::default_random_engine						m_generator;
	
	std::vector<std::unique_ptr<Particles>>			m_explodedParticles;
	std::vector<sf::CircleShape>					m_force;	// make class for m_force

	bool											m_FrictionOn;
	bool											m_GravityOn;
	bool											m_AirResistanceOn;
	float											m_forceWaveVelocity;
public:
	ParticleManage() : m_forceWaveVelocity{ 0 }, m_FrictionOn{ false }, m_GravityOn{ false }, m_AirResistanceOn{ false } {};

	void explode(sf::Vector2i, sf::PrimitiveType, sf::Vector2f randomRange = sf::Vector2f(0.0f, 0.0f), int amount = 1000);
	void vacuum(sf::Vector2i);

	void createParticles(sf::PrimitiveType type = sf::Points, sf::Vector2i mousePosition = sf::Vector2i(0, 0), int amount = 1000);
	void setParticleExpandAttributes(std::vector<std::unique_ptr<Particles>>&, sf::Vector2i, sf::PrimitiveType, sf::Vector2f randomRange = sf::Vector2f(0.0f, 0.0f), int amount = 1000);
	
	std::vector<std::unique_ptr<Particles>>& getExplodedParticles() { return m_explodedParticles; }

	void particlePush(const std::vector<std::tuple<size_t, size_t, size_t>>&, bool collision = false);
	void createForceWave(sf::Vector2i, float radius = 1.0f);
	void forceWaveExpand(float, sf::Vector2f);
	const auto isForceWaveCollided();

	void setForceVelocity(float velocity) { m_forceWaveVelocity = velocity; }
	const float getForceVelocity() { return m_forceWaveVelocity; }

	void TurnOnForce(bool, ParticleSettings::Forces);
	void forceUpdate();
	void applyGravityForce(sf::Vector2f force);
	void applyAirResistance(float coefficent = 0.0001f);
	void applyFriction(float mi = 0.01);


	float getRandomFloat(float, float);
	void update(float dt);
	void draw(sf::RenderWindow&);
};

// predefinedExplosion zamiast explosion
// predefinedVacuum

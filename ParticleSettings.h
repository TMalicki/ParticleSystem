#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class ParticleSettings
{
private:
	sf::Vector2f				 m_force;
	sf::Vector2f				 m_acceleration;
	sf::Vector2f				 m_velocity;
	sf::Vector2f				 m_direction;
	sf::Vector2f				 m_position;

	float						 m_lifeTimeMs;
	float						 m_mass;
public:
	ParticleSettings(sf::Vector2f force = { 0.0,0.0 }, sf::Vector2f position = { 0.0f, 0.0f }, sf::Vector2f direction = { 0.0f, 0.0f }) :
		m_force{ force },
		m_acceleration{},
		m_velocity{}, 
		m_position{ position },
		m_direction{ direction },
		m_lifeTimeMs{ 5000.0f },
		m_mass{ 1.0f } {};
	
	enum class Forces { Gravity, AirResistance, External, Friction };
	
	void applyForce(sf::Vector2f force, Forces forceType = Forces::External, float constant = 0.0f );
	//void applyForce(sf::Vector2f force, sf::Vector2f direction);
	const sf::Vector2f& getForce() { return m_force; }

	void setPosition(sf::Vector2f position) { m_position = position; }
	const sf::Vector2f& getPosition() { return m_position; }

	void setDirection(sf::Vector2f direction) { this->m_direction = direction; }
	const sf::Vector2f& getDirection() { return m_direction; }
	const sf::Vector2f getDirectionFromVelocity();

	void setMass(float mass) { this->m_mass = mass; }
	const float getMass() { return m_mass; }

	void setAcceleration(sf::Vector2f acceleration) { this->m_acceleration = acceleration; }
	const sf::Vector2f& getAcceleration() { return m_acceleration; }

	void setVelocity(sf::Vector2f velocity) { this->m_velocity = velocity; }
	const sf::Vector2f& getVelocity() { return m_velocity; }

	void setLifeTime(float time) { m_lifeTimeMs = time; }
	void reduceLifeTime(float dt) { m_lifeTimeMs -= dt; }
	const float getLifeTime() { return m_lifeTimeMs; }
};

// sort by angle : 0, 14, 46, 78, 90, 96, 106 ??





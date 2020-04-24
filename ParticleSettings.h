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

	float						 m_mass;
	sf::Color					 m_color;
public:
	ParticleSettings(sf::Vector2f acc = { 0.0,0.0 }, sf::Vector2f vel = { 0.0,0.0 }, sf::Vector2f dir = { 0.0f, 0.0f }) :
		m_acceleration(acc),
		m_velocity{ vel }, 
		m_direction{ dir }, 
		m_color{ sf::Color::White },
		m_mass(1.0f) {};
	
	enum class Forces { Gravity, AirResistance, External, Friction };
	
	sf::Vector2f getDirectionFromVelocity();

	void applyForce(sf::Vector2f force, Forces forceType, float constant = 0.0f);
	const sf::Vector2f& getForces() { return m_force; }

	void setMass(float mass) { this->m_mass = mass; }
	const float getMass() { return m_mass; }

	void setAcceleration(sf::Vector2f acceleration) { this->m_acceleration = acceleration; }
	const sf::Vector2f& getAcceleration() { return m_acceleration; }

	void setVelocity(sf::Vector2f velocity) { this->m_velocity = velocity; }
	const sf::Vector2f& getVelocity() { return m_velocity; }

	void setDirection(sf::Vector2f direction) { this->m_direction = direction; }
	const sf::Vector2f& getDirection() { return m_direction; }

	void setColor(sf::Color color) { this->m_color = color; }
	const sf::Color& getColor() { return m_color; }
};



// sort by angle : 0, 14, 46, 78, 90, 96, 106
// check where force is : dir == (-0.56, 0.4)
// 




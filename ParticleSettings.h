#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class ParticleSettings
{
private:
	sf::Vector2f m_velocity;
	sf::Vector2f m_direction;
	sf::Color	 m_color;
public:
	ParticleSettings(sf::Vector2f vel = { 0.0,0.0 }, sf::Vector2f dir = { 0.0f, 0.0f }) : m_velocity{ vel }, m_direction{ dir }, m_color{ sf::Color::White } {};
	
	void setVelocity(sf::Vector2f velocity) { this->m_velocity = velocity; }
	const sf::Vector2f& getVelocity() { return m_velocity; }

	void setDirection(sf::Vector2f direction) { this->m_direction = direction; }
	const  sf::Vector2f& getDirection() { return m_direction; }

	void setColor(sf::Color color) { this->m_color = color; }
	const sf::Color& getColor() { return m_color; }
};
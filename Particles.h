#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticleSettings.h"

using std::vector;

class Particles
{
private:
	sf::VertexArray m_particles;
	vector<ParticleSettings> m_particleAttributes;

	sf::PrimitiveType m_type;
public:
	//Particles(int amount);
	Particles(long int amount = 100, sf::Vector2f position = { 0.0,0.0 }, sf::Vector2f velocity = { 0.0,0.0 }, sf::Vector2f direction = { 0,0 }, sf::PrimitiveType tempType = sf::Points);

	void setType(sf::PrimitiveType type) { this->m_type = type; }
	const sf::PrimitiveType& getType() { return m_type; }

	const sf::VertexArray& getParticle() { return m_particles; }
	const vector<ParticleSettings>& getParticleAttributes() { return m_particleAttributes; }
	void move(sf::Vector2i);
	///void explode(sf::Vector2i);
	//void move(sf::Vector2i mousePosition);
	void update(double);


	void setParticleAttributes(size_t index = 0, sf::Vector2f position = { 0.0, 0.0 }, sf::Vector2f velocity = { 0.0, 0.0 }, sf::Vector2f direction = { 0.0, 0.0 });
};


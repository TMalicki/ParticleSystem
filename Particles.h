#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticleSettings.h"

class Particles
{
private:							
	std::vector<ParticleSettings>	m_particleAttributes;	
	sf::VertexArray					m_particleVertex;

	sf::PrimitiveType				m_type;
	static sf::Vector2f				s_activeAreaSize;
public:
	Particles(long int amount = 100, sf::Vector2f position = { 0.0,0.0 }, sf::Vector2f velocity = { 0.0,0.0 }, 
				sf::Vector2f direction = { 0,0 }, sf::PrimitiveType tempType = sf::Points);

	static void setActiveArea(sf::Vector2f activeArea) { s_activeAreaSize = activeArea; }
	const size_t getParticlesAmount() { return m_particleVertex.getVertexCount(); }
	const sf::VertexArray& getParticleVertex() const { return m_particleVertex; }
	void moveTowardsPoint(sf::Vector2f);

	std::vector<ParticleSettings>& getParticleAttributes() { return m_particleAttributes; };
	void setParticleAttributes(size_t index = 0, sf::Vector2f position = { 0.0, 0.0 }, sf::Vector2f velocity = { 0.0, 0.0 }, sf::Vector2f direction = { 0.0, 0.0 });

	void setType(sf::PrimitiveType type) { this->m_type = type; }
	const sf::PrimitiveType& getType() { return m_type; }

	void setParticleColor(size_t index, sf::Color color) { m_particleVertex[index].color = color; }
	const auto& getParticleColor(size_t index) { return m_particleVertex[index].color; }

	void draw(sf::RenderWindow& window) { window.draw(m_particleVertex); }
	void update(float);
};


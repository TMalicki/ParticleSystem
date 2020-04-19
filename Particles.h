#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticleSettings.h"

using std::vector;
class Particles
{
private:							
	vector<ParticleSettings>	  m_particleAttributes;	
	sf::VertexArray				  m_particlesVertex;

	sf::PrimitiveType			  m_type;
	static sf::Vector2f			  s_activeAreaSize;
public:
	Particles(long int amount = 100, sf::Vector2f position = { 0.0,0.0 }, sf::Vector2f velocity = { 0.0,0.0 }, sf::Vector2f direction = { 0,0 }, sf::PrimitiveType tempType = sf::Points);

	static void setActiveArea(sf::Vector2f activeArea) { s_activeAreaSize = activeArea; }

	void setType(sf::PrimitiveType type) { this->m_type = type; }
	const sf::PrimitiveType& getType() { return m_type; }

	const sf::VertexArray& getParticleVertex() const { return m_particlesVertex; }
	vector<ParticleSettings>& getParticleAttributes() { return m_particleAttributes; }	// czy przez to ze vector jest bez referencji elementów nie ma czasami kopiowania?
	void setParticleAttributes(size_t index = 0, sf::Vector2f position = { 0.0, 0.0 }, sf::Vector2f velocity = { 0.0, 0.0 }, sf::Vector2f direction = { 0.0, 0.0 });

	void setParticleColor(size_t i, sf::Color color) { m_particlesVertex[i].color = color; }
	const size_t getParticlesAmount() { return m_particlesVertex.getVertexCount(); }
	void update(float);
};


﻿#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticleSettings.h"

class Particles
{
private:							
	std::vector<ParticleSettings>	m_particleAttributes;	
	std::vector<sf::Vertex>			m_particleVertex;

	sf::PrimitiveType				m_type;

	bool m_GravityOn;
public:
	Particles(long int amount = 100, sf::Vector2f position = { 0.0,0.0 }, sf::Vector2f velocity = { 0.0,0.0 }, 
				sf::Vector2f direction = { 0,0 }, sf::PrimitiveType tempType = sf::Points);
	
	std::vector<sf::Vertex>& getParticleVertex() { return m_particleVertex; }
	std::vector<ParticleSettings>& getParticleAttributes() { return m_particleAttributes; };
	void setParticleAttributes(size_t index = 0, sf::Vector2f position = { 0.0, 0.0 }, sf::Vector2f velocity = { 0.0, 0.0 }, sf::Vector2f direction = { 0.0, 0.0 });///?
	void setParticleAttributesN(size_t index = 0, sf::Vector2f position = { 0.0, 0.0 }, sf::Vector2f velocity = { 0.0, 0.0 });///?

	void getDirectionTowardsPoint(sf::Vector2f);

	void TurnOnForce(bool, ParticleSettings::Forces);
	void forceUpdate();
	void applyForce(sf::Vector2f force, ParticleSettings::Forces forceType = ParticleSettings::Forces::External, float constant = 0.0f);
	void applyGravityForce(sf::Vector2f force);
	void applyAirResistance(float coefficent = 0.0001f);
	void applyFriction(float mi = 0.01);

	void setMass(size_t index, float mass) { m_particleAttributes[index].setMass(mass); }

	void setType(sf::PrimitiveType type) { this->m_type = type; }
	const sf::PrimitiveType& getType() { return m_type; }

	void setParticleColor(size_t index, sf::Color color) { m_particleVertex[index].color = color; }
	const auto& getParticleColor(size_t index) { return m_particleVertex[index].color; }

	void draw(sf::RenderWindow& window) { window.draw(m_particleVertex.data(),m_particleVertex.size(),sf::Points); }
	void update(float);
};


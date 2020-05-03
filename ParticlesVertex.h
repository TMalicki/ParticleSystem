#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticlesInterface.h"

//template <typename T>
class ParticlesVertex : public ParticlesInterface
{
private:							
	std::vector<sf::Vertex>			m_particleVertex;
	//std::vector<T>			m_particleVertex;
public:
	ParticlesVertex(long int amount = 100, sf::Vector2f position = { 0.0,0.0 });
	void setParticleAttributesN(size_t index, sf::Vector2f position, sf::Vector2f velocity);

	virtual void calculateVerticesPosition();
	virtual const sf::Color& getParticleColor(size_t index);
	virtual void setParticleColor(size_t index, sf::Color color);
	virtual void setDirectionTowardsPoint(sf::Vector2f);

	virtual void draw(sf::RenderWindow& window);
	virtual void update(float);

	std::vector<sf::Vertex>& getParticleVertex() { return m_particleVertex; }
};


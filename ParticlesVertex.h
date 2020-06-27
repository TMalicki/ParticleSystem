#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticlesInterface.h"

//template <typename T>
class ParticlesVertex : public ParticlesInterface
{
private:							
	std::vector<sf::Vertex>				m_particleVertex;
public:
	ParticlesVertex(long int amount = 100, sf::Vector2f position = { 0.0,0.0 });
	ParticlesVertex(long int amount = 100, std::vector<sf::Vector2f> position = {});
	//ParticlesVertex(const ParticlesVertex&) { std::cout << "ParticlesVertex kopia"; };

	virtual const sf::Vector2f getPosition(size_t index) { return m_particleVertex.at(index).position; }
	virtual const std::vector<sf::Vector2f> getPosition();
	virtual void setPosition(size_t index, sf::Vector2f position) { m_particleVertex.at(index) = position; };
	virtual void setPosition(std::vector<sf::Vector2f> positions);

	virtual void eraseParticles(std::vector<size_t> index);
	virtual void eraseParticles(std::vector<ParticleSettings>::iterator);

	virtual std::vector<sf::Color> getColor();
	virtual void setColor(std::vector<sf::Color>);
	virtual void reduceColorOpacity(int);

	void setParticleAttributes(size_t index, sf::Vector2f position, sf::Vector2f velocity);
	virtual const sf::Color& getParticleColor(size_t index) { return m_particleVertex.at(index).color; }
	virtual void setParticleColor(size_t index, sf::Color color) { m_particleVertex.at(index).color = color; }
	virtual void setDirectionTowardsPoint(sf::Vector2f);

	virtual void draw(sf::RenderWindow& window) { window.draw(m_particleVertex.data(), m_particleVertex.size(), sf::Points); }
	virtual void update(float);

	std::vector<sf::Vertex>& getParticleVertex() { return m_particleVertex; }
};


#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticlesInterface.h"

//template <typename T>
class ParticlesCircle : public ParticlesInterface
{
private:
	std::vector<sf::CircleShape>			m_particleCircle;
public:
	ParticlesCircle(long int amount = 100, sf::Vector2f position = { 0.0,0.0 });

	virtual const sf::Vector2f getPosition(size_t index) { return m_particleCircle.at(index).getPosition(); }
	virtual const std::vector<sf::Vector2f> getPosition();
	virtual void setPosition(size_t index, sf::Vector2f position) { m_particleCircle.at(index).setPosition(position); };
	virtual void setPosition(std::vector<sf::Vector2f> positions);

	virtual void eraseParticles(std::vector<size_t> index);
	virtual void eraseParticles(std::vector<ParticleSettings>::iterator);
	virtual void fadingParticle(float dt);

	virtual void setColor(std::vector<sf::Color>);

	void setParticleAttributes(size_t index, sf::Vector2f position, sf::Vector2f velocity);
	virtual const sf::Color& getParticleColor(size_t index) { return m_particleCircle.at(index).getFillColor(); }
	virtual void setParticleColor(size_t index, sf::Color color) { m_particleCircle.at(index).setFillColor(color); }
	virtual void setDirectionTowardsPoint(sf::Vector2f);

	virtual void draw(sf::RenderWindow& window);
	virtual void update(float);

	std::vector<sf::CircleShape>& getParticleCircle() { return m_particleCircle; }
};


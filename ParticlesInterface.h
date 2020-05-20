#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticleSettings.h"

class ParticlesInterface
{
protected:
	std::vector<ParticleSettings>	m_particleAttributes;	
private:
	float							m_maxVelocity;
public:
	ParticlesInterface(long int amount = 100, sf::Vector2f position = { 0.0,0.0 });
	//ParticlesInterface(const ParticlesInterface&) { std::cout << "ParticlesInterface kopia"; };

	virtual std::vector<sf::Color> getColor() = 0;
	virtual void setColor(std::vector<sf::Color>) = 0;

	virtual const std::vector<sf::Vector2f> getPosition() = 0;
	virtual void setPosition(std::vector<sf::Vector2f>) = 0;

	virtual void eraseParticles(std::vector<size_t>) = 0;
	virtual void eraseParticles(std::vector<ParticleSettings>::iterator) = 0;


	virtual void setDirectionTowardsPoint(sf::Vector2f) = 0;
	virtual void reduceColorOpacity(int) = 0;
	virtual void fadingParticle(float dt) = 0;

	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void update(float) = 0;

	virtual const std::vector<sf::Vector2f> getVelocity();
	virtual void setVelocity(std::vector<sf::Vector2f>);

	size_t getParticlesAmount() { return m_particleAttributes.size(); }
	const std::vector<ParticleSettings>& getParticleAttributes() { return m_particleAttributes; }

	void setMaxVelocity(float max) { m_maxVelocity = max; }
	float getMaxVelocity() { return m_maxVelocity; }
		
	void setDirection(sf::Vector2f);
	void setDirection(size_t, sf::Vector2f);
	void setDirection(std::vector<sf::Vector2f>);

	void applyForce(size_t index, sf::Vector2f force, ParticleSettings::Forces forceType = ParticleSettings::Forces::External, float constant = 0.0f);
	void applyForce(sf::Vector2f force, ParticleSettings::Forces forceType = ParticleSettings::Forces::External, float constant = 0.0f);
	void applyForce(std::vector<sf::Vector2f>, ParticleSettings::Forces forceType = ParticleSettings::Forces::External, float constant = 0.0f);
	void applyGravityForce(sf::Vector2f force);
	void applyAirResistance(float coefficent = 0.0001f);
	void applyFriction(float mi = 0.01);

	void toErase();
	void reduceLifeTime(float dt);

	void setMass(std::vector<float> masses);
};


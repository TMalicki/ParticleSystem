#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Particle.h"

using std::vector;

class ParticleSystem
{
private:
	//Particle particle;
	sf::VertexArray particles;
public:
	ParticleSystem(int amount);
	ParticleSystem(const ParticleSystem& copyParticles);
	ParticleSystem& operator=(const ParticleSystem& assignParticles);

	~ParticleSystem() { std::cout << "Usunieto ParticleSystem\n"; };

	sf::VertexArray& getParticles() { return particles; }
	void move(sf::Vector2i);
	void explode(sf::Vector2i);
};


#endif

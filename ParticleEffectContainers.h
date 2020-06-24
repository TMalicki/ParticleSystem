#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include "ParticlesInterface.h"
#include "ParticleSettings.h"

class ParticleEffectContainers
{
private:
	std::vector<std::vector<std::shared_ptr<ParticlesInterface>>>	m_particleContainer;

	std::vector<std::shared_ptr<ParticlesInterface>>			m_explodedParticles;
	std::vector<std::shared_ptr<ParticlesInterface>>			m_emiterParticles;
	std::vector<std::shared_ptr<ParticlesInterface>>			m_tunnelParticles;
public:
	ParticleEffectContainers() : m_particleContainer{ m_explodedParticles, m_emiterParticles, m_tunnelParticles } {};

	std::vector<std::shared_ptr<ParticlesInterface>>& getByIndex(size_t index)
	{ 
		return m_particleContainer[index]; 
	}
};
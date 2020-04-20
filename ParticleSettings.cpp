#include "ParticleSettings.h"

void ParticleSettings::applyForce(sf::Vector2f force)
{
	m_force = (force / m_mass);
	m_acceleration += sf::Vector2f{ m_force.x * m_direction.x, m_force.y * m_direction.y }; 
}

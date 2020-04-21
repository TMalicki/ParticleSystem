#include "ParticleSettings.h"

sf::Vector2f ParticleSettings::getDirectionFromVelocity()
{
	//auto actualPosition = getParticleVertex()[i].position;
	//auto newDirectionVector = sf::Vector2f{ goalPosition.x - actualPosition.x, goalPosition.y - actualPosition.y };
	auto newDirectionMagnitude = sqrt(pow(m_velocity.x, 2) + pow(m_velocity.y, 2));
	auto newDirection = sf::Vector2f{};

	if (newDirectionMagnitude == 0.0f) newDirection = sf::Vector2f{ 0.0f,0.0f };
	else newDirection = sf::Vector2f{ m_velocity.x / newDirectionMagnitude, m_velocity.y / newDirectionMagnitude };

	//m_direction = newDirection;

	return newDirection;
}

void ParticleSettings::applyForce(sf::Vector2f force, Forces forceType)
{
	static int x = 0;
	if(forceType == Forces::Gravity) 
	{
		m_force = force;
	}
	else if (forceType == Forces::AirResistance)
	{
		float c = 0.0001f;
		auto velocityMagnitude = sqrt(pow(m_velocity.x, 2) + pow(m_velocity.y, 2));
		//auto direction = getDirectionFromVelocity();

		m_direction = -getDirectionFromVelocity();
		m_force = pow(velocityMagnitude, 2) * c * sf::Vector2f{1.0f, 1.0f};
	}
	else if (forceType == Forces::Friction)
	{
		auto normal = 1.0f;
		auto mi = 0.01f;
		//auto direction = -getDirectionFromVelocity();
		m_direction = -getDirectionFromVelocity();

		m_force = normal * mi * sf::Vector2f{ 1.0f, 1.0f };
	}
	else
	{
		m_force = (force / m_mass);
	}
	m_acceleration += sf::Vector2f{ m_force.x * m_direction.x, m_force.y * m_direction.y }; 

	x++;
	if (x == 100) {
		std::cout << "Force: " << m_force.x << " " << m_force.y << "\n";
		std::cout << "Direction: " << m_direction.x << " " << m_direction.y << "\n";
		std::cout << "Velocity: " << m_velocity.x << " " << m_velocity.y << "\n";
		x = 0;
	}
}

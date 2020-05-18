#pragma once
#include <iostream>
#include "ParticlesInterface.h"
#include <SFML/Graphics.hpp>

class EmiterEffect
{
private:
	bool														m_EmiterOn;
	std::vector<sf::CircleShape>								m_EmiterObject;
	std::vector<size_t>											m_EmiterCounter;
	std::vector<float>											m_EmiterTimer;
public:
	EmiterEffect() : m_EmiterObject(), m_EmiterOn(false), m_EmiterCounter(), m_EmiterTimer() {};

	void createEmiter(sf::Vector2i);
	void updateEmiter(float);
	bool getEmiterLogic() { return m_EmiterOn; }
	void setEmiterLogic(bool state) { m_EmiterOn = state; }
	const std::vector<sf::Vector2f> getEmitersPositions();

	void draw(sf::RenderWindow& window);
};
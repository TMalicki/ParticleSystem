#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"

class windowSettings
{
private:
	sf::Vector2f m_activeWindowSize;
	sf::Vector2f m_optionWindowSize;

public:
	windowSettings(sf::Vector2f windowSize, float border = 300.0f) : m_activeWindowSize(windowSize.x - border, windowSize.y), m_optionWindowSize(border, windowSize.y) {};
	const auto getActiveWindowSize() { return m_activeWindowSize; }
	const auto getOptionWindowSize() { return m_optionWindowSize; }
};
#pragma once

#include <SFML/Graphics.hpp>

class Event
{
private:
	sf::Event event;
public:
	Event() : event{} {};
	void updateEvent(sf::RenderWindow&);
};
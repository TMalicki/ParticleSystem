#pragma once

#include "GameSettings.h"
#include "LoadWorld.h"

#include "Draw.h"
#include "Event.h"

class Game
{
private:
	sf::RenderWindow window;

	Event event;
	Draw draw;
	GameSettings gameSettings;
	LoadWorld loadWorld;
public:
	Game(unsigned int tileWidth = 40, unsigned int tileHeight = 40, unsigned int tileAmountHorizon = 30, unsigned int tileAmountVertical = 24)
		: gameSettings(tileWidth, tileHeight, tileAmountHorizon, tileAmountVertical)
		, loadWorld(gameSettings.getSettings())
		, window{ sf::VideoMode(tileWidth * tileAmountHorizon, tileHeight * tileAmountVertical), "ALPAKATOR 2.0" }, draw{} {};
	
	//void initializeGame() { event.getWindow(window), draw.getWindow(window) }; // add it if necessary (for not sending window with every new method of those classes)
	void run();
	void load();
};
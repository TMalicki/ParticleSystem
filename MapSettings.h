#pragma once

class MapSettings
{
protected:
	const unsigned int tileAmountHorizon;
	const unsigned int tileAmountVertical;
public:
	MapSettings(unsigned int tileAmountHorizon = 20, unsigned int tileAmountVertical = 16) : tileAmountHorizon{ tileAmountHorizon }, tileAmountVertical{ tileAmountVertical } {}

	const sf::Vector2u getTileAmount() { return sf::Vector2u{ tileAmountHorizon, tileAmountVertical }; };
};
#include "Draw.h"


// that class should be observer of LoadWorld class (observator design pattern)

void Draw::draw(sf::RenderWindow& window)
{
	window.clear();

	for (size_t i = 0; i < renderedMap.size(); i++)
	{
		for (size_t j = 0; j < renderedMap[i].size(); j++)
		{
			window.draw(renderedMap[i][j]->getSprite());
		}
	}
	
	window.display();
}

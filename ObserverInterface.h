#pragma once
#include <SFML/Graphics.hpp>

class ObserverInterface
{
private:
public:
	ObserverInterface() {};
	virtual ~ObserverInterface() {};

	virtual void updateMap(std::vector<std::vector<std::shared_ptr<MapTile>>>&) = 0;
};
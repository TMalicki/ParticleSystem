#pragma once

#include "MapGenerator.h"
#include "MapTile.h"
#include "GameSettings.h"
#include "SubjectInterface.h"
#include "ObserverInterface.h"
#include "RandomWalkAlgorithm.h"

class LoadWorld : public SubjectInterface
{
private:
	std::shared_ptr<MapGenerator> actualLevel;
	MapTile mapTile;

	std::vector<std::vector<std::shared_ptr<MapTile>>> mapTiles;
	std::vector<ObserverInterface*> listObserver;
public:
	LoadWorld(std::shared_ptr<GameSettings> gameSettingsPtr) : actualLevel(std::make_shared<RandomWalkAlgorithm>(gameSettingsPtr->getTileAmount())), mapTile{ gameSettingsPtr->getTileSize() }
	{ };
	virtual ~LoadWorld() {};

	void loadWorld();
	void prepareTheWorld();
	void generateWorld();

	// observer design pattern methods
	virtual void attach(ObserverInterface* observer) { listObserver.push_back(observer); };
	virtual void detach(ObserverInterface* observer);
	virtual void notify() { std::for_each(listObserver.begin(), listObserver.end(), [&](ObserverInterface* observer) { observer->updateMap(mapTiles); }); };
	
};
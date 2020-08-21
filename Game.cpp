#include "Game.h"


void Game::run()
{
    while (window.isOpen())
    {
        event.updateEvent(window);
        draw.draw(window);
    }
}

void Game::load()
{
    loadWorld.attach(&draw);
    loadWorld.loadWorld();
    loadWorld.detach(&draw);   
}

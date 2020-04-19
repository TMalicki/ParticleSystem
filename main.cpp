#include <iostream>
#include <SFML/Graphics.hpp>
#include "windowSettings.h"
#include "ParticleManage.h"
#include "Timer.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ParticleSystem");
    auto start = std::chrono::high_resolution_clock::now();;
    
    sf::Vector2f windowSize = sf::Vector2f{ static_cast<float>(window.getSize().x) - 320, static_cast<float>(window.getSize().y) };

    windowSettings windowSettings(windowSize);

    auto windowOptionSize = windowSettings.getOptionWindowSize();
    
    ParticleManage particles;
    particles.setActiveAreaSize(windowSettings.getActiveWindowSize());

    while (window.isOpen())
    {
        auto dt = getTime(start);
 
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
     
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            particles.vacuum(mousePosition);
        }


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    particles.explode(mousePosition, sf::Points, sf::Vector2f(1.0, 5.0), 5000);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    //particles.vacuum(mousePosition);
                }
            }
        }

        particles.update(dt);

        window.clear();
        particles.draw(window);    
        window.display();
    }
    return 0;
}
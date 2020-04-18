#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticleManage.h"
#include "Timer.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ParticleSystem");
    auto start = std::chrono::high_resolution_clock::now();;

    ParticleManage particles;

    while (window.isOpen())
    {
        auto dt = getTime(start);
 
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f windowSize = sf::Vector2f{ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) };
     
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
                    particles.vacuum(mousePosition);
                }
            }
        }

        particles.update(dt, windowSize);

        window.clear();
        particles.draw(window);    
        window.display();
    }
    return 0;
}
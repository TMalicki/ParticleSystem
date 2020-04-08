#include <iostream>
#include <SFML/Graphics.hpp>
#include "Particles.h"
#include "Timer.h"
#include <math.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");

    auto start = std::chrono::high_resolution_clock::now();
    Particles particles(50000,sf::Vector2f(0.0,0.0),sf::Vector2f(1.0,2.0));
    
    double counter {1.0};
    for (size_t i = 0; i < particles.getParticle().getVertexCount(); i++)
    {
        particles.setParticleAttributes(i, { 900.0,500.0 }, { 1.0,1.0 }, { counter, counter });
        //counter += 1.0;
    }

    while (window.isOpen())
    {
        auto dt = getTime(start);
        std::cout << dt << "\n";

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        //particles.move(mousePosition);
      
        particles.update(dt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) 
            {
                
            }
        }


        window.clear();

      
        window.draw(particles.getParticle());
        
        
        window.display();
    }

    return 0;
}
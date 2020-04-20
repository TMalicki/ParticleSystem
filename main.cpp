#include <iostream>
#include <SFML/Graphics.hpp>
#include "windowSettings.h"
////#include "ParticleManage.h"
#include "Timer.h"
#include "Particles.h"



/*
Źle jest i nie działa, przerobić trzeba co nie co. Spróbować najpierw zrobić działającą klasę Particles. Wygenerować 
je w mainie normalnie i później sprawdzić jak reagują na podążanie za myszką. Jeśli atrybuty te będą zmieniały 
się przyzwoicie i będzie dobra hermetyzacja danych to wtedy spróbować zaprogramować dopiero w ParticleManage klasie
odpowiednie algorytmy zachowań.
*/




int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ParticleSystem");
    auto start = std::chrono::high_resolution_clock::now();;
    
    sf::Vector2f windowSize = sf::Vector2f{ static_cast<float>(window.getSize().x) - 320, static_cast<float>(window.getSize().y) };

    windowSettings windowSettings(windowSize);

    auto windowOptionSize = windowSettings.getOptionWindowSize();
    
    
   /* */ Particles particles(1, sf::Vector2f(100.0, 100.0));
   /* */ particles.setActiveArea(sf::Vector2f{ 1500.0f, 1080.0f });

   //// ParticleManage particles;
   //// particles.setActiveAreaSize(windowSettings.getActiveWindowSize());

    while (window.isOpen())
    {
        auto dt = getTime(start);
 
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        ////////////////////////////////////////////
        /// TEST

        /**/ particles.moveTowardsPoint(static_cast<sf::Vector2f>(mousePosition));
        
        /// TEST
        ////////////////////////////////////////////

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            ////particles.vacuum(mousePosition);
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
                  ////  particles.explode(mousePosition, sf::Points, sf::Vector2f(-3.0, 3.0), 1000);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                   //// particles.vacuum(mousePosition);
                }
            }
        }

        /* */particles.update(dt);
        window.clear();
        /* */particles.draw(window);
       // particles.draw(window);    
        window.display();
    }
    return 0;
}
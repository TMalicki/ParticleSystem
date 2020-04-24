#include <iostream>
#include <TGUI/TGUI.hpp>
//#include <SFML/Graphics.hpp>
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

/*OGARNĄĆ JEDNOSTKI!*/
/*POPRAWIĆ SIŁY - SZCZEGÓLNIE DZIAŁANIE ARGUMENTU DLA METODY APPLYAIRRESISTANCE*/

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    sf::Vector2f windowSize = sf::Vector2f{ 1920.0f, 1080.0f };
    float settingWindowWidth = 320;

    sf::RenderWindow window(sf::VideoMode(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y)), "ParticleSystem");
    windowSettings windowSettings(window);
    windowSettings.loadGUI();
    
    Particles particles(1, sf::Vector2f(100.0, 100.0)); 
    particles.setMass(0, 3.0f);
       
    //// ParticleManage particles;
    //// particles.setActiveAreaSize(windowSettings.getActiveWindowSize());
    auto& x = windowSettings.getObjectsUI();

   while (window.isOpen())
   {
       auto dt = getTime(start);

       sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

       /**/ //particles.applyAirResistance();

       particles.applyFriction();
       
       if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
       {
           particles.getDirectionTowardsPoint(static_cast<sf::Vector2f>(mousePosition));
           particles.applyForce(sf::Vector2f{ 0.1f , 0.1f });
           ////particles.vacuum(mousePosition);
       }
       if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
       {
           particles.applyGravityForce(sf::Vector2f{ 0.0f, 0.02f });
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
            windowSettings.updateGUI(event);
            x.at(0)->connect("Checked", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::Gravity); });
            x.at(0)->connect("Unchecked", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::Gravity); });
        }

        /* */particles.update(dt);
            windowSettings.transitionParticle(particles.getParticleVertex());
             window.clear();
        /* */particles.draw(window);
        /**/ windowSettings.drawGUI();
        /**/ //gui.draw();
       // particles.draw(window);    
        window.display();
    }
    return 0;
}
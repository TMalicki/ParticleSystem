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
/*DODAĆ STEROWALNE PRZYCISKI DO ZMIANY USTAWIEŃ*/
/*POPRAWIĆ SIŁY - SZCZEGÓLNIE DZIAŁANIE ARGUMENTU DLA METODY APPLYAIRRESISTANCE*/

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ParticleSystem");
    tgui::Gui gui{ window };
    auto start = std::chrono::high_resolution_clock::now();;
    
    sf::Vector2f windowSize = sf::Vector2f{ static_cast<float>(window.getSize().x) - 320, static_cast<float>(window.getSize().y) };

    windowSettings windowSettings(windowSize);

    auto windowOptionSize = windowSettings.getOptionWindowSize();

   /* */ Particles particles(10, sf::Vector2f(100.0, 100.0));
   /* */ particles.setActiveArea(sf::Vector2f{ 1500.0f, 1080.0f });
         
   /**/ //particles.setMass(0, 3.0f);
   /**/    for (size_t i = 0; i < 10; i++)
   /**/    {
   /**/        particles.setMass(i, static_cast<float>(i + 1));
   /**/    }
       
   //// ParticleManage particles;
   //// particles.setActiveAreaSize(windowSettings.getActiveWindowSize());
   gui.loadWidgetsFromFile("GUI/Side.txt");
   auto x = gui.getWidgets();

   while (window.isOpen())
   {
       auto dt = getTime(start);

       sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

       /**/ //particles.applyAirResistance(1000.f);

       /**/ particles.applyFriction();
       if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
       {
           /**/ particles.getDirectionTowardsPoint(static_cast<sf::Vector2f>(mousePosition));
           /**/ particles.applyForce(sf::Vector2f{ 0.1f , 0.1f });
           ////particles.vacuum(mousePosition);
       }
       if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
       {
           /**/ particles.applyGravityForce(sf::Vector2f{ 0.0f, 0.02f });
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
            gui.handleEvent(event);
         //   x.at(0)->connect("Checked", [&]() { particles.applyGravityForce(sf::Vector2f{ 0.0f,0.02f }); });
        }

        /* */particles.update(dt);
        window.clear();
        /* */particles.draw(window);
        /**/gui.draw();
       // particles.draw(window);    
        window.display();
    }
    return 0;
}
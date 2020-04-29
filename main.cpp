#include <iostream>
#include <TGUI/TGUI.hpp>
#include "windowSettings.h"
#include "ParticleManage.h"
#include "Timer.h"
#include "Particles.h"

// forceWave push should be force dependent

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    sf::Vector2f windowSize = sf::Vector2f{ 1920.0f, 1080.0f };
    float settingWindowWidth = 320;

    sf::RenderWindow window(sf::VideoMode(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y)), "ParticleSystem");
    windowSettings windowSettings(window);
    windowSettings.loadGUI();
         
    /**/ParticleManage particlesMan;
    particlesMan.setActiveArea(windowSettings.getActiveWindowSize());

   while (window.isOpen())
   {
       auto dt = getTime(start);

       sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
       
       if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
       {
           for (auto& particle : particlesMan.getExplodedParticles())
           {
               /**/particle->setDirectionTowardsPoint(static_cast<sf::Vector2f>(mousePosition));
               /**/particle->applyForce(sf::Vector2f{ 0.1f , 0.1f });
           }
           ////particles.vacuum(mousePosition);
       }
       if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
       {
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
                    particlesMan.explode(mousePosition, sf::Points, sf::Vector2f(-3.0, 3.0), 1);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {

                }
            }
            windowSettings.updateGUI(event);
        }

        /**///windowSettings.updateLogicGUI(particles);
        windowSettings.updateLogicGUI(particlesMan);

        /**/particlesMan.update(dt);
        
        /**///windowSettings.transitionParticles(particlesMan.getExplodedParticles());/////
        windowSettings.reboundBorders(particlesMan.getExplodedParticles());
        windowSettings.colorParticlesByVelocity(particlesMan);

       window.clear();
       
       /**/particlesMan.draw(window);
       
       /**/windowSettings.drawGUI();
      

       window.display();
    }
    return 0;
}
#include <iostream>
#include <TGUI/TGUI.hpp>
#include "windowSettings.h"

#include "ParticleManage.h"
#include "Timer.h"

// turn on randomness walk particle - needed
// after all particles on the screen erased and new explosion wanna be made - error occured
// friction should be working only in y direction? Or should i choose in which direction that works (x and y, or just one of them)
// emiter sometimes is erasing while using erasingBorders when it should not happen
int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    sf::Vector2f windowSize = sf::Vector2f{ 1920.0f, 1080.0f };
    float settingWindowWidth = 320;

    sf::RenderWindow window(sf::VideoMode(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y)), "ParticleSystem");
    windowSettings windowSettings(window);
    windowSettings.loadGUI();


     ParticleManage particlesMan;
     particlesMan.setActiveArea(windowSettings.getActiveWindowSize());

    while (window.isOpen())
    {
        auto dt = getTime(start);

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {       
            for (auto& particle : particlesMan.getExplodedParticles())
            {
                particle->setDirectionTowardsPoint(static_cast<sf::Vector2f>(mousePosition));
                particle->applyForce(sf::Vector2f{ 0.1f , 0.1f });
            }
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
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
                    if (particlesMan.getParticleEffect() == ParticleManage::ParticleEffect::Explode)
                    {
                        particlesMan.explode(mousePosition, sf::Vector2f(-3.0, 3.0), 1000);
                    }
                    else if (particlesMan.getParticleEffect() == ParticleManage::ParticleEffect::Emiter)
                    {
                        particlesMan.emitter(mousePosition, sf::Vector2f(-3.0, 3.0), 1000);
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {

                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                int temp = static_cast<int>(particlesMan.getParticleEffect());
                int max = static_cast<int>((ParticleManage::ParticleEffect::NUMBER_OF_EFFECTS));
                if (event.mouseWheelScroll.delta > 0)
                {
                    if (temp < max)
                    {
                        particlesMan.setEffectType(static_cast<ParticleManage::ParticleEffect>((temp + 1)));
                    }
                    else
                    {
                        particlesMan.setEffectType(static_cast<ParticleManage::ParticleEffect>((0)));
                    }
                }
                else if (event.mouseWheelScroll.delta < 0)
                {
                    if (temp > 0)
                    {
                        particlesMan.setEffectType(static_cast<ParticleManage::ParticleEffect>((temp - 1)));
                    }
                    else
                    {
                        particlesMan.setEffectType(static_cast<ParticleManage::ParticleEffect>((max)));
                    }
                }
             }
            windowSettings.updateGUI(event);
        }
        particlesMan.update(dt);
        windowSettings.updateLogicGUI(windowSettings, particlesMan);

        windowSettings.colorParticlesByVelocity(particlesMan);

        window.clear();

        particlesMan.draw(window);  
        windowSettings.drawGUI();

        window.display();
    }
    return 0;
}
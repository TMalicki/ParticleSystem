#include <iostream>
#include <SFML/Graphics.hpp>
#include "ParticleManage.h"
#include "Timer.h"

// sf::VertexArray m_particles;	// maybe list will be better? try that, also maybe vector<VertexArray> for explosion?
// stare byc odpychane w zaleznosci od odleglosci od miejsca wybuchu
int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
    
    auto start = std::chrono::high_resolution_clock::now();;

    /*
  //  sf::VertexArray timePlotY(sf::PrimitiveType::Points, 1000);
  //  int timePlotX{};
 
    for (int i = 0; i < timePlotY.getVertexCount(); i++)
    {
        timePlotY[i].color = sf::Color::Yellow;
    }
    */

    //Particle particles(50000,sf::Vector2f(0.0,0.0),sf::Vector2f(2.0,2.0));
    ParticleManage particles;
    size_t xx = 0;
    while (window.isOpen())
    {
        auto dt = getTime(start);
       // std::cout << dt << "\n";
        /*
        timePlotY[timePlotX].position = sf::Vector2f(static_cast<double>(timePlotX), dt + 5);
        if(timePlotX < 1000)
            timePlotX++;
            */

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        //particles.move(mousePosition);
     
        sf::Vector2f windowSize = sf::Vector2f{ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) };
        particles.update(dt, windowSize);
  
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) 
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    xx++;
                    particles.explode(mousePosition, sf::Points, sf::Vector2f(1.0,5.0), 10000);
                   // particles.explosionForce(mousePosition);
                   // particles.forcePush();
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    particles.vacuum();
                }
            }
        }


        window.clear();

        particles.draw(window);   
      //  window.draw(timePlotY);
        
        window.display();
    }

    auto x = timePlot();
    for (float i : x)
    {
        std::cout << i << std::endl;
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "math.hpp"
#include "UserInterface.hpp"
#include "VerletParticle.hpp"
#include "PhysicsSolver.hpp"
#include "GeneralFunctions.hpp"
#include "CollisionGrid.hpp"
#include "Spring.hpp"
#include <iomanip>

using namespace sf;

int main ()
{
   // Window Dimensions
   unsigned int winWidth = 1920;
   unsigned int winHeight = 1080;
 
   unsigned int CENTERX = winWidth/2;
   unsigned int CENTERY = winHeight/2;

   // Simulation Settings
   float dampFactor = 10;
   float stiffness = 100;
   float ParticleRadius = 10;
   int substeps = 7;

   float springSpacing = 10;
   float k = 50;
   float dampening = 10;

   // Spawn Grid or Spring Grid
   int rowAmt = 10;
   int columnAmt = 10;


   // Window Init
   RenderWindow win(VideoMode::getDesktopMode(), "Nico's Simulation", Style::Fullscreen);
   win.setFramerateLimit(500);

   // CLOCKS AND TIME VARIABLES **************************
   sf::Clock clock;
   float deltaTime;
   float timeMultiplier = 14;

   sf::Clock spawnClock;
   float tempTime;

   sf::Clock spawnClock2;
   float tempTime2;

   Clock elapsedClock;
   float elapsedTime;
   /**/

   // FPS DISPLAY VARIABLES *******************************
   sf::Font font;
   font.loadFromFile("C:\\SFML_fonts\\Kanit-Regular.ttf");

   sf::Text fpsText;
   fpsText.setFont(font);
   fpsText.setFillColor(Color::Green);
   fpsText.setPosition(Vector2f(winWidth * 0.93, 10));
   /**/

   // CONSTRAINT VARIABLES ******************************
   CircleShape constraintShape;
   float constraintRadius = 480;
   constraintShape.setRadius(constraintRadius);
   constraintShape.setPosition(CENTERX - constraintRadius, CENTERY - constraintRadius);
   constraintShape.setOutlineThickness(5);
   constraintShape.setOutlineColor(Color(200,200,200));
   constraintShape.setPointCount(60);
   constraintShape.setFillColor(Color(0, 0, 0));


   /**/

   PhysicsSolver Solver = PhysicsSolver(winWidth, winHeight, win, substeps, constraintRadius);

   //Solver.createParticleSpringGrid(10, 10, 20, 150, 4000);

   // for(int row = 0; row < rowAmt; row++)
   // {
   //    for(int column = 0; column < columnAmt; column++)
   //    {
   //       float randSize = 5; //rand() % 10 + 8;
   //       Vector2f iPos = Vector2f((Solver.simWidth/2 + column * springSpacing) - (springSpacing*columnAmt)/2, (Solver.simHeight/2 + row * springSpacing) - 150);
   //       Solver.addParticle(iPos, Vector2f(0, 0), randSize, randSize, false, false);
   //    }
   // }


   // Program Loop
   while(win.isOpen())
   {
      // Closes window and polls events
      sf::Event event;
      while(win.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
         {
            win.close();
         }
         if (event.type == Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape)
            {
               win.close();
            }
         }
         if(event.type == Event::MouseButtonPressed)   
         {
            float randSize = rand() % 10 + 20;
            //Solver.addParticle(Vector2f(Mouse::getPosition().x, Mouse::getPosition().y), Vector2f(0, 0), 5, 10, false, false);
         }         
      }

      tempTime = spawnClock.getElapsedTime().asSeconds() * 100;
      tempTime2 = spawnClock2.getElapsedTime().asSeconds() * 100;
      elapsedTime = elapsedClock.getElapsedTime().asSeconds() * 1000;

      // FPS Update Code ************************************
      deltaTime = clock.restart().asSeconds();
      int fps = 1/deltaTime;
      
      if(tempTime > 10)
      {
         if (fps < 60)
         {
            fpsText.setFillColor(Color(200, 90, 10));
         }
         else
         {
            fpsText.setFillColor(Color::Green);
         }
         fpsText.setString(std::to_string(int(fps)) + " fps");
      }

      /**/

      if(spawnClock2.getElapsedTime().asSeconds() > 0.008 && Keyboard::isKeyPressed(Keyboard::Space))
      {
         float randSize = 10; //rand() % 10 + 8;
         Vector2f iPos = Vector2f(100, 100);
         Vector2f vec = Vector2f(Mouse::getPosition().x, Mouse::getPosition().y) - Vector2f(100, 100);
         vec = multVectConst(normalize(vec), 5);

            Solver.addParticle(Vector2f(100, 130), vec, randSize, randSize, false, false, Rainbow(elapsedTime, 6000));
            Solver.addParticle(Vector2f(100, 150), vec, randSize, randSize, false, false, Rainbow(elapsedTime, 6000));
            Solver.addParticle(Vector2f(100, 170), vec, randSize, randSize, false, false, Rainbow(elapsedTime, 6000));
            Solver.addParticle(Vector2f(100, 190), vec, randSize, randSize, false, false, Rainbow(elapsedTime, 6000));
         
         spawnClock2.restart();
      }

      // Rendering
      win.clear();
      //win.draw(constraintShape);

      if(elapsedTime > 50)
      {
         Solver.update(deltaTime);
      }
      
      win.draw(fpsText);
      win.display();
   }
   return 0;
}
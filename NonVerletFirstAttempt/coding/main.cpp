
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "math.hpp"



using namespace sf;

class particle
{     
   public:
      sf::Vector2f acc;
      sf::Vector2f vel;
      sf::Vector2f pos;
      sf::CircleShape shape;

      float timeMultiplier;
      float radius;

      particle(sf::Vector2f acceleration, sf::Vector2f velocity, sf::Vector2f position, float radius, float timeMult, float color)
      {
         acc = acceleration;
         vel = velocity;
         pos = position;
         radius = radius;
         timeMultiplier = timeMult;

         if(color == 1)
         {
            shape.setFillColor(sf::Color{250, 150, 60});
         }
         else if(color == 2)
         {
            shape.setFillColor(sf::Color{250, 130, 50});
         }
         else
         {
            shape.setFillColor(sf::Color{250, 180, 70});
         }

         shape.setRadius(radius);
         shape.setPosition(pos);
      }
      
      void addForce(Vector2f force)
      {
         acc += force; // add / mass bc f = ma and a = f / m
      }

      void update(float deltaTime)
      {
         vel += acc * deltaTime * timeMultiplier;// * deltaTime * timeMultiplier;
         pos += vel * deltaTime * timeMultiplier;
         

         shape.setPosition(pos.x, pos.y);
      }

      void blackHole(float origin_x, float origin_y)
      {
         Vector2f pullForce = normalize(Vector2f((origin_x - pos.x),  (origin_y - pos.y)));
         pullForce.x *= 10;
         pullForce.y *= 10;
         addForce(pullForce);
      }

      void dragForce(float drag_amount)
      {
         float speed = mag(vel);

         float drag = speed * speed * drag_amount;

         Vector2f vel_normalized = normalize(vel);
         float drag_x = vel_normalized.x * -drag;
         float drag_y = vel_normalized.y * -drag;

         vel.x += drag_x;
         vel.y += drag_y;
      }

      void BoundsCollision(float width, float height, float damp)
      {
         if(pos.y > height - radius*2)
         {
            pos.y =  height - radius*2;
            vel.y *= -1 * damp;
         }
         else if(pos.y < radius)
         {
            pos.y =  radius;
            vel.y *= -1 * damp;
         }
         else if(pos.x > width - radius*2)
         {
            pos.x = width - radius*2;
            vel.x *= -1 * damp;
         }
         else if(pos.x < radius)
         {
            pos.x = radius;
            vel.x *= -1 * damp;
         }
      }
};

int main ()
{
   // Settings
   //Vector2f init_pos = ;

   // Window Dimensions
   int win_width = 1600;
   int win_height = 1000;
   
   // Window Init
   sf::RenderWindow win = sf::RenderWindow(sf::VideoMode(win_width, win_height), "Sandbox Program");
   win.setFramerateLimit(230);


   // Time and frame rate
   sf::Clock clock;
   float deltaTime;
   float timeMultiplier = 14;
   
   // FPS display
   sf::Font font;
   font.loadFromFile("C:\\SFML_fonts\\Kanit-Regular.ttf");

   sf::Text fps_text;
   fps_text.setFont(font);
   fps_text.setFillColor(Color::Green);


   // PARTICLE ARRAY
   std::vector<particle> particle_array = {};

   // Initialize particles
   for(int i = 0; i < 10000; i++)
   {
      // Get random positions
      int rand_x = abs(rand() % (win_width - 500));
      int rand_y = abs(rand() % (win_height - 500));
      // Add particle to array
      particle_array.push_back(particle(sf::Vector2f(rand()%20, 5), sf::Vector2f(0, 0), sf::Vector2f(rand_x, rand_y), 1 + rand() % 1, timeMultiplier, rand() % 3));
   }


   // Program Loop
   while (win.isOpen())
   {
      // Update deltaTime by resting clock which returns clock value
      deltaTime = clock.restart().asSeconds();

      float fps = 1 / deltaTime;
      fps_text.setString("FPS:  " + std::to_string(fps));

      // Checks if window close event happens then it closes program
      sf::Event event;
      while(win.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
         {
            win.close();
         }            
      }
      
      
      // Clear screen and display particles----------------------------------------------
      win.clear();

      // Particle Handling
      for(particle &ptc:particle_array)
      {
         // ptc.addForce(sf::Vector2f(0,  0.1));
         // sf::Mouse::getPosition().x, sf::Mouse::getPosition().y

         ptc.blackHole(win_width/2, win_height/2);

         ptc.update(deltaTime);

         ptc.dragForce(0.01);

         ptc.BoundsCollision(win_width, win_height, 0.5);

         win.draw(ptc.shape);
      }


      

      // Color fps accordingly
      if (fps < 60)
      {
         fps_text.setFillColor(Color::Red);
      }
      else
      {
         fps_text.setFillColor(Color::Green);
      }


      win.draw(fps_text);


      // Displays particles
      win.display();
      //---------------------------------------------------------------------------------
   }

   return 0;
}

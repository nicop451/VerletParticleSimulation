#ifndef SPRING_HPP
#define SPRING_HPP

#include <SFML/Graphics.hpp>
#include "VerletParticle.hpp"

class Spring
{
   public:
      float k;
      float dampFactor;
      float restLength;
      VerletParticle &p1;
      VerletParticle &p2;

      Spring(VerletParticle &initP1, VerletParticle &initP2, float initRestLength, float initK, float initDampFactor);

      void update();

      void display(RenderWindow& win);
};

#endif
#ifndef VERLETPARTICLE_HPP
#define VERLETPARTICLE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "math.hpp"

using namespace sf;

struct VerletParticle
{
   float mass;
   float radius;
   unsigned int id;
   bool isLocked;
   bool isHidden;
   Vector2f pos;
   Vector2f oldPos;
   Vector2f sumofForces;
   CircleShape shape;
   Color ptcColor;

   VerletParticle(Vector2f iPos, Vector2f iVel, float iMass, float iRadius, bool iIsLocked, bool iIsHidden, unsigned int iId, Color iColor);

   void applyForce(Vector2f force);

   void display(sf::RenderWindow &win);

   void update(Vector2f gravity, float deltaTime);

   void constrain(int simWidth, int simHeight, float constraintRadius, float dampening);
};

#endif // VERLETPARTICLE_HPP

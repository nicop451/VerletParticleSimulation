#ifndef PHYSCISSOLVER_HPP
#define PHYSICSSOLVER_HPP

#include <SFML/Graphics.hpp>
#include "math.hpp"
#include "VerletParticle.hpp"
#include "Spring.hpp"
#include "CollisionGrid.hpp"
#include <cmath>
#include <iostream>

using namespace sf;


struct PhysicsSolver
{
   int substeps;
   float simWidth;
   float simHeight;
   float constraintRadius;
   float k;
   float dampening{0.1};
   RenderWindow& win;
   Vector2f gravity{Vector2f(0, 700)};
   Clock elapsedTime;

   std::vector<VerletParticle> particleArray;
   std::vector<Spring> springArray;

   Grid grid;

   PhysicsSolver(float iSimWidth, float iSimHeight, RenderWindow& iWin, int substeps, float iConstraintRadius);

   void update(float deltaTime);

   // PARTICLE FUNCTIONS
   void addParticle(Vector2f initPos, Vector2f initVel, float mass, float radius, bool iIsLocked, bool iIsHidden, Color iColor);

   void updatePositions(float deltaTime);

   void fixPtcOvelap(VerletParticle& p1, VerletParticle& p2);

   void HandleCollisions();

   void displayParticles(RenderWindow& win);

   // SPRING FUNCTIONS
   void addSpring(VerletParticle& p1, VerletParticle& p2, float initRestLength, float iK, float iDamp);

   void createParticleSpringGrid(int rows, int columns, float springSpacing, float iK, float iDamp);

   void addSpringForces();
   
   void displaySprings(RenderWindow& win);

   // COLLISION GRID FUNTCIONS
   void Updategrid();

   void checkCellCollisions(Cell& c1, Cell& c2);
};

#endif 
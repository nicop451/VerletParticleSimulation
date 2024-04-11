#include "VerletParticle.hpp"

VerletParticle::VerletParticle(Vector2f iPos, Vector2f iVel, float iMass, float iRadius, bool iIsLocked, bool iIsHidden, unsigned int iId, Color iColor):isHidden(iIsHidden), id(iId), ptcColor(iColor)
{
    pos = iPos;
    oldPos = pos - iVel;
    radius = iRadius;
    mass = iMass;
    isLocked = iIsLocked;
    isHidden = iIsHidden;
    shape.setPosition(pos.x, pos.y);
    shape.setRadius(radius);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setFillColor(ptcColor);
    Vector2f temp = pos + Vector2f(0, 500);
}

void VerletParticle::applyForce(Vector2f force)
{
    sumofForces = addVect(sumofForces, force);
}

void VerletParticle::display(sf::RenderWindow &win)
{
    shape.setPosition(pos.x, pos.y);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setRadius(radius);
    win.draw(shape);
}

void VerletParticle::update(Vector2f gravity, float deltaTime)
{
   if(!isLocked)
   {
      Vector2f vel = pos - oldPos;
      vel = multVectConst(vel, 1 - 10 * deltaTime * deltaTime);
      Vector2f acc = sumofForces / mass;
      acc += gravity;
      acc = acc * (deltaTime * deltaTime);

      oldPos = pos; // Save current position

      vel += acc;
      pos += vel; // update position

      sumofForces = Vector2f(0, 0); // reset Forces
   }
   shape.setPosition(pos.x, pos.y); // change the position of the shape
    
}

void VerletParticle::constrain(int simWidth, int simHeight, float constraintRadius, float dampening)
{
   if(!isLocked)
   {
      Vector2f vel = pos - oldPos;

      // BOUNDS COLLISIONS ****************
      if (pos.y > simHeight - radius)
      {
         pos.y = simHeight - radius;
         oldPos.y = pos.y + vel.y * dampening;
      }
      if (pos.y < radius)
      {
         pos.y = radius;
         oldPos.y = pos.y + vel.y * dampening;
      }
      if (pos.x > simWidth - radius)
      {
         pos.x = simWidth - radius;
         oldPos.x = pos.x + vel.x * dampening;
      }
      if (pos.x < radius)
      {
         pos.x = radius;
         oldPos.x = pos.x + vel.x * dampening;
      }

      // Vector2f centerPos = Vector2f(simWidth / 2 , simHeight / 2);
      // Vector2f toCenter = pos - centerPos;
      // float distToCenter = mag(toCenter);
      // Vector2f toCenterNormalized = normalize(toCenter);
      // if (distToCenter > constraintRadius - radius)
      // {
      //    float n = constraintRadius / distToCenter;
      //    //oldPos = pos;
      //    pos = centerPos + toCenterNormalized * (constraintRadius - radius);
      // }
   }
}

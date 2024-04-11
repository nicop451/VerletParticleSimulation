#include "Spring.hpp"
#include "math.hpp"

Spring::Spring(VerletParticle &initP1, VerletParticle &initP2, float initRestLength, float initK, float initDampFactor):p1(initP1), p2(initP2), restLength(initRestLength), k(initK), dampFactor(initDampFactor)
{
   p1 = initP1;
   p2 = initP2;

   restLength = initRestLength;
   k = initK;
}

void Spring::update()
{
   Vector2f SpringVector = p2.pos - p1.pos;
   Vector2f SprVecNormalized = normalize(SpringVector);

   float x = mag(SpringVector) - restLength;
   

   Vector2f SpringForce1 = SprVecNormalized * x * k;

   SpringForce1 = multVectConst(SpringForce1, 1000);

   Vector2f SpringForce2 = multVectConst(SpringForce1, -1);

   Vector2f vel1 = p1.pos - p1.oldPos;

   Vector2f vel2 = p2.pos - p2.oldPos;

   Vector2f dampForce1 = multVectConst(dot(vel2-vel1, SpringVector) * SprVecNormalized, dampFactor);
   Vector2f dampForce2 = multVectConst(dampForce1, -1);


   Vector2f finalForce1 = SpringForce1 + dampForce1;
   Vector2f finalForce2 = SpringForce2 + dampForce2;


   p1.applyForce(finalForce1);
   p2.applyForce(finalForce2);
}

void Spring::display(RenderWindow &win)
{
   sf::Vertex line[] = {
      sf::Vertex(sf::Vector2f(p1.pos.x, p1.pos.y), sf::Color::White),
      sf::Vertex(sf::Vector2f(p2.pos.x, p2.pos.y), sf::Color::White)
   };
   win.draw(line, 2, sf::Lines);
}
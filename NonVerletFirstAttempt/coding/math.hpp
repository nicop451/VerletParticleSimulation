#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using namespace sf;

Vector2f addVect(Vector2f v1, Vector2f v2)
{
   float x = v1.x + v2.x;
   float y = v1.y + v2.y; 

   return Vector2f(x, y);
}

float mag(Vector2f vec)
{
   float a = vec.x;
   float b = vec.y; 

   a *= a;
   b *= b;

   return sqrtf(a + b);
}

Vector2f normalize(Vector2f vec)
{
   float vec_mag = mag(vec);
   float vec_x = vec.x / vec_mag;
   float vec_y = vec.y / vec_mag;
   Vector2f finalVec = Vector2f(vec_x, vec_y);
   return finalVec;
}
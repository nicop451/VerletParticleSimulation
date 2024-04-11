#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

Color Rainbow(float length, float maxLength)
{
    float i = (length * 255 / maxLength);
    float r = round(sin(0.024 * i + 0) * 127 + 128);
    float g = round(sin(0.024 * i + 2) * 127 + 128);
    float b = round(sin(0.024 * i + 4) * 180 + 200);
    return Color(r, g, b);
}
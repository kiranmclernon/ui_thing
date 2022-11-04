#include "geometery.hpp"
#include <algorithm>



Vect2f::Vect2f(const sf::Vector2f& sfml_vect) : x(sfml_vect.x), y(sfml_vect.y) {}

sf::Vector2f Rectangle::topLeft() const
{
    return sf::Vector2f(x_min, y_min);
}


sf::Vector2f Rectangle::topRight() const
{
    return sf::Vector2f(x_max, y_min);
}

sf::Vector2f Rectangle::bottomLeft() const
{
    return sf::Vector2f(x_min, y_max);
}

sf::Vector2f Rectangle::bottomRight() const
{
    return sf::Vector2f(x_max, y_max);
}


float Rectangle::getWidth() const
{
    return x_max - x_min;
}

float Rectangle::getHeight() const
{
    return y_max - y_min;
}











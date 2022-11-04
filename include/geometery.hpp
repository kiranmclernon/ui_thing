#include <SFML/Graphics.hpp>
#include <array>
#include <utility>
#include <iostream>
#include <exception>
#include <algorithm>
#pragma once

struct Vect2f
{
    Vect2f(const sf::Vector2f& sfml_vect);
    float x, y;
    constexpr Vect2f(const float& x_, const float& y_) : x(x_), y(y_) {}
    constexpr Vect2f() : x(0), y(0) {}
};

constexpr Vect2f operator+(const Vect2f& lhs , const Vect2f& rhs)
{
    return Vect2f(lhs.x + rhs.x, lhs.y + rhs.y);
}


struct RectangleBoundaryHelper
{
    float x_max, x_min, y_max, y_min;
    constexpr RectangleBoundaryHelper(std::tuple<float, float, float, float>&& boundaries);
    constexpr RectangleBoundaryHelper();
};

struct Rectangle : public RectangleBoundaryHelper {
    std::pair<Vect2f, Vect2f> corners;
    constexpr Rectangle(const Vect2f& c1, const Vect2f& c2);
    constexpr Rectangle();
    constexpr Rectangle transform(const Vect2f& transform) const;
    float getHeight() const;
    float getWidth() const;
    sf::Vector2f topRight() const;
    sf::Vector2f topLeft() const;
    sf::Vector2f bottomLeft() const;
    sf::Vector2f bottomRight() const;
};


std::ostream& operator<<(std::ostream& stream, const Rectangle& r) 
{
    stream << "Rectangle: \n";
    stream << "x_min: " << r.x_min;
    stream << "\nx_max: "<< r.x_max;
    stream << "\ny_min: " << r.y_min;
    stream << "\ny_max: "<< r.y_max;
    return stream;
}

constexpr bool intersectionChecker(const Rectangle& r1, const Rectangle& r2)
{
  return r1.x_min < r2.x_max
      && r1.x_max > r2.x_min
      && r1.x_min < r2.y_max
      && r1.x_max > r2.y_min;
}


constexpr std::tuple<float, float, float, float> getRectangleBoundaries(const Vect2f& c1, const Vect2f& c2)
{
    if(c1.x == c2.x or c1.y == c2.y)
        throw std::runtime_error("Not valid corners");
    std::pair<float, float> x, y;
    return {std::max(c1.x,c2.x), std::min(c1.x, c2.x), std::max(c1.y, c2.y), std::min(c1.y,c2.y)};
}

constexpr RectangleBoundaryHelper::RectangleBoundaryHelper(std::tuple<float, float, float, float>&& b)
    : x_max(std::get<0>(b)), x_min(std::get<1>(b)), y_max(std::get<2>(b)), y_min(std::get<3>(b)) {}

constexpr RectangleBoundaryHelper::RectangleBoundaryHelper() : x_max(0), x_min(0), y_max(0), y_min(0) {}

constexpr Rectangle::Rectangle(const Vect2f& c1, const Vect2f& c2) : corners({c1,c2}), 
          RectangleBoundaryHelper(getRectangleBoundaries(c1, c2))
{}



std::ostream& operator<<(std::ostream& stream, const Vect2f& v)
{
    stream << "(x: " << v.x << " y : " << v.y << ")";
    return stream;
}




constexpr Rectangle Rectangle::transform(const Vect2f &transform) const
{
    Vect2f c1_new = corners.first + transform;
    Vect2f c2_new = corners.second + transform;
    return Rectangle(c1_new, c2_new);
}


constexpr Rectangle::Rectangle() {}





#include <window.hpp>
#include <array>
#include <utility>
#include <vector>

Rectangle makeRectangle(const sf::RenderWindow& window)
{
    float x = (float)window.getSize().x;
    float y = (float)window.getSize().y;
    return Rectangle(Vect2f(0,0), Vect2f(x,y));
}


BackGround::BackGround(const sf::RenderWindow& window) : Element(makeRectangle(window)) 
{}


void BackGround::claim_handle(sf::RenderWindow &rw, Element*& handle_owner) {}

void BackGround::draw(sf::RenderTarget &target, sf::RenderStates states) const {}

void BackGround::eventHandler(sf::RenderWindow &rw, Element*& handle_owner) {}



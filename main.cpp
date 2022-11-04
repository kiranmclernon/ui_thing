#include <iostream>
#include <geometery.hpp>
#include "element.hpp"
#include "window.hpp"






sf::Font font;

ButtonStyle slider_button_style(
        sf::Color(70, 99, 98),
        sf::Color(136, 150, 171),
        sf::Color(197, 213, 228)
        );
TextStyle slider_text_style(
        20, sf::Color(177,229, 242), font, "slider demonstration"
        );

SliderStyle slider_style(
        slider_button_style, sf::Color::Transparent, sf::Color(177, 229, 242)
        );


int main()
{
    font.loadFromFile("/Users/kiranmclernon/Documents/Projects/PhysicsCode/faster/resources/Fonts/sf-pro-display-cufonfonts/SFPRODISPLAYBOLD.OTF");
    slider_text_style.setTextFont(font);
    //slider_text_style.setTextColor(sf::Color(177, 229, 242));
    //slider_text_style.setTextString("slider demonstration");
TextBoxStyle slider_text_box_style(
        slider_text_style,
        sf::Color::Transparent
        );
    
    TextStyle button_text = slider_text_style;
    button_text.setTextString("HI JACK");
    button_text.setTextColor(sf::Color(219, 90, 186));
    BaseButtonStyle bs1 = {slider_button_style, button_text};

    constexpr Rectangle r3(Vect2f(200,600), Vect2f(1200, 300));
    constexpr Rectangle r1(Vect2f(100, 100), Vect2f(500, 200));
    BaseRectangleButton b1(r1, bs1);
    Slider s1(r3, slider_style, 0, 10, slider_text_box_style);
    constexpr WindowSize size = {1920, 1080};
    Window<size, r3, r1> wWindow(s1, b1);    
    wWindow.loop();
    return 0;
}

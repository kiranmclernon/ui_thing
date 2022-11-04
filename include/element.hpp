#include <geometery.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>
#include <memory>
#pragma once

enum ElementState
{
    PRESSED = 0,
    HOVER = 1,
    RELAX = 2
};

class Element : public sf::Drawable
{
public:
    ElementState state = RELAX;
    virtual void claim_handle(sf::RenderWindow& rw, Element*& handle_owner) = 0;
    sf::Vector2f getCentre() const;
    Rectangle getBoundingBox() const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    virtual ~Element() {}
    virtual void update() {}
protected:
    Element(const Rectangle& bounding_box_);
    Rectangle bounding_box;
    sf::Vector2f centre;
    bool contains(const sf::Vector2f& point) const noexcept;
    virtual void eventHandler(sf::RenderWindow& rw, Element*& handle_owner) = 0;
    void OwnershipHandler(Element*& handle_owner);
};



std::ostream& operator<<(std::ostream& os, const sf::Vector2f& v)
{
    os << "x: " << v.x << " y : " << v.y;
    return os;
}


std::ostream& operator<<(std::ostream& os, const sf::Color& c)
{
    os << "Red: " << (int)c.r << " Green: " << (int)c.g << " Blue: " << (int)c.b;
    return os;
}


struct ButtonStyle
{
    ButtonStyle(const sf::Color& pressed_color_, const sf::Color& hover_color_, const sf::Color& rest_color_);
    sf::Color pressed_color;
    sf::Color hover_color;
    sf::Color rest_color;
    ButtonStyle();
};


// Why does this exist??? 
// It makes initialising text easier
// When i refactor ill probably get rid
struct TextStyle : public sf::Drawable
{
    TextStyle(const uint& text_size_, const sf::Color& text_color_, 
            const sf::Font& text_font_, const std::optional<std::string>& text_string_ = std::nullopt);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    TextStyle();   
    const sf::Text& getTextRef() const;
    uint getTextSize() const;
    sf::Font getTextFont() const;
    sf::Color getTextColor() const ;
    std::string getTextString() const;
    sf::Vector2f getTextPosition() const;
    void setTextSize(const uint& size_);
    void setTextFont(const sf::Font& font_);
    void setTextColor(const sf::Color& color_);
    void setTextString(const std::string& string_);
    void setTextPosition(const sf::Vector2f& postion_);
    void centre();
private:
    sf::Text text;
    uint text_size;
    sf::Font text_font;
    sf::Color text_color;
    std::string text_string;
};


struct BaseButtonStyle
{
    ButtonStyle button_style;
    TextStyle text_style;

};


class ButtonBase : public Element
{
public:
    BaseButtonStyle getStyle() const;
protected:
    ButtonBase(const Rectangle& bounding_box, const BaseButtonStyle& style_); 
    BaseButtonStyle style;
    bool pressed = false;
    virtual void on_release() = 0;
    virtual void setColor(const sf::Color& color) = 0;
};


class BaseRectangleButton : public ButtonBase
{
public:
    BaseRectangleButton(const Rectangle& bounding_box_, const BaseButtonStyle& style_);
    void claim_handle(sf::RenderWindow& rw, Element*& handle_owner);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update();
private:
    sf::RectangleShape rect;
    void eventHandler(sf::RenderWindow& rw, Element*& handle_owner);
    void setColor(const sf::Color& color);
    void on_release();
};



struct TextBoxStyle
{
    TextStyle text_style;
    sf::Color back_drop_color;
    TextBoxStyle(const TextStyle& text_style_, const sf::Color& back_drop_color_);
    TextBoxStyle();
};

class TextBox : public sf::Drawable
{
public:
    TextBox(const TextBoxStyle& style_, const Rectangle& bounding_box_);
    TextBox(const TextBoxStyle& style_, const Rectangle& parent_box, const float& proportion);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getCentre() const;
    void setPosition(const sf::Vector2f& position_);
//private:
    Rectangle bounding_box;
    TextBoxStyle style;
    sf::RectangleShape back_drop;
    sf::Vector2f centre;
};


struct SliderStyle
{
    sf::Color end_circle_color;
    sf::Color bar_color;
    ButtonStyle progress_circle_style;
    SliderStyle();
    SliderStyle(const ButtonStyle& progress_circle_style_, const sf::Color& bar_color_, const sf::Color& end_circle_color_);
};

class Slider : public Element
{
public:
    Slider(const Rectangle& bounding_box_, const SliderStyle& style_, const float min, const float& max,
            TextBoxStyle title_style);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void claim_handle(sf::RenderWindow& rw, Element*& handle_owner);
    Slider(const Slider& other);
    void update();
//private:
    void sharedConstructorFacilities(const SliderStyle& style_, const float min, const float& max,
            TextBoxStyle title_style);
    bool inProgressCircle(const sf::Vector2f& point);
    void eventHandler(sf::RenderWindow& rw, Element*& handle_owner);
    // These are only pointers bc constructors a bitch
    float value;
    float width;
    float range;
    float max_value;
    float min_value;
    SliderStyle style;
    std::unique_ptr<TextStyle> min_value_text;
    std::unique_ptr<TextStyle> max_value_text;
    std::unique_ptr<TextBox> title;
    ButtonStyle progress_circle_style;
    sf::CircleShape left_side_circle;
    sf::CircleShape right_side_circle;
    sf::RectangleShape bar;
    sf::RectangleShape progress_bar;
    sf::CircleShape progress_circle;
};

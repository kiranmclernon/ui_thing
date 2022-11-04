#include <element.hpp>
#include <exception>
#include <cmath>
#include <sstream>
#include <iomanip>


void centreText(sf::Text& text)
{
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top  + textRect.height/2.0f);
}

void setRectSize(sf::RectangleShape& rect, const Rectangle& bounding_box)
{
    sf::Vector2f sideLengths(bounding_box.x_max-bounding_box.x_min, bounding_box.y_max - bounding_box.y_min);
    rect.setSize(sideLengths);
}

void setRectOrigin(sf::RectangleShape& rect)
{
    sf::Vector2f size = rect.getSize();
    rect.setOrigin(sf::Vector2f(size.x/2.0f, size.y/2.0f));
}

void setCircleOrigin(sf::CircleShape& circ)
{
    circ.setOrigin(circ.getRadius(), circ.getRadius());
}

sf::Vector2f getBoxCentre(const Rectangle& bounding_box)
{
    return sf::Vector2f((bounding_box.x_max + bounding_box.x_min) / 2,
            (bounding_box.y_max + bounding_box.y_min) / 2);
}

void setCircleX(sf::CircleShape& circ, const float& x)
{
    circ.setPosition(sf::Vector2f(x, circ.getPosition().y));
}

bool Element::contains(const sf::Vector2f& point) const noexcept
{
    const Rectangle &b = bounding_box;
    if(point.x < b.x_max && point.x > b.x_min && point.y < b.y_max && point.y > b.y_min)
        return true;
    else
        return false;
}





sf::Vector2f Element::getCentre() const {return centre;}

Rectangle Element::getBoundingBox() const {return bounding_box;}

Element::Element(const Rectangle& bounding_box_) : bounding_box(bounding_box_)
{
    centre = getBoxCentre(bounding_box);
}

void Element::OwnershipHandler(Element *& handle_owner)
{
    if(handle_owner->getCentre() != centre)
    {
        if(handle_owner->state == PRESSED)
        {
            return;
        }
        else if(handle_owner-> state == RELAX)
        {
            handle_owner = this;
        }
        else if (handle_owner-> state == HOVER)
        {
            handle_owner = this;
        }
    }
}

ButtonStyle::ButtonStyle(const sf::Color& pressed_color_, const sf::Color& hover_color_, const sf::Color& rest_color_)
    : pressed_color(pressed_color_), hover_color(hover_color_), rest_color(rest_color_) {}


ButtonStyle::ButtonStyle() : pressed_color(sf::Color::Red), rest_color(sf::Color::Green), hover_color(sf::Color::Blue) {}

TextStyle::TextStyle(const uint& text_size_, const sf::Color& text_color_, 
            const sf::Font& text_font_, const std::optional<std::string>& text_string_)
    : text_size(text_size_), text_color(text_color_), text_font(text_font_)
{
    text.setFont(text_font);
    text.setFillColor(text_color);
    text.setCharacterSize(text_size);
    if(text_string_)
    {
        text_string = *text_string_;
        text.setString(text_string);
        centreText(text);
    }
}

void TextStyle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(text);
}

TextStyle::TextStyle()
{
    text_font.loadFromFile("/Users/kiranmclernon/Documents/Projects/PhysicsCode/faster/resources/Fonts/sf-pro-display-cufonfonts/SFPRODISPLAYBOLD.OTF");
    text_string = "default_text";
    text.setString(text_string);
    text.setFont(text_font);
    text_size = 20;
    text.setCharacterSize(20);
    text_color = sf::Color::White;
    text.setFillColor(text_color);
    centreText(text);

}

const sf::Text& TextStyle::getTextRef() const {return text;}

uint TextStyle::getTextSize() const {return text_size;}

sf::Font TextStyle::getTextFont() const {return text_font;}

sf::Color TextStyle::getTextColor() const {return text_color;}

std::string TextStyle::getTextString() const {return text_string;}

sf::Vector2f TextStyle::getTextPosition() const {return text.getPosition();}

void TextStyle::setTextSize(const uint& size_)
{
    text_size = size_;
    text.setCharacterSize(text_size);
}

void TextStyle::setTextFont(const sf::Font& font_)
{
    text_font = font_;
    text.setFont(text_font);
}

void TextStyle::setTextColor(const sf::Color& color_)
{
    text_color = color_;
    text.setFillColor(text_color);
}


void TextStyle::setTextString(const std::string& string_)
{
    text_string = string_;
    text.setString(text_string);
}


void TextStyle::setTextPosition(const sf::Vector2f& position_)
{
    text.setPosition(position_);
}

void TextStyle::centre()
{
    centreText(text);
}

ButtonBase::ButtonBase(const Rectangle& bounding_box_, const BaseButtonStyle& style_) 
    : Element(bounding_box_), style(style_) {}


BaseButtonStyle ButtonBase::getStyle() const
{
    return style;
}


BaseRectangleButton::BaseRectangleButton(const Rectangle& bounding_box_, const BaseButtonStyle& style_)
    : ButtonBase(bounding_box_, style_)
{
    setRectSize(rect, bounding_box);
    setRectOrigin(rect);
    rect.setPosition(centre);
    style.text_style.setTextPosition(centre);
    setColor(style.button_style.rest_color);
}



sf::Vector2f vector_i_to_f(const sf::Vector2i& v)
{
    return sf::Vector2f(v.x, v.y); 
}


void BaseRectangleButton::claim_handle(sf::RenderWindow& rw, Element*& handle_owner)
{
    sf::Vector2f mousePos = vector_i_to_f(sf::Mouse::getPosition(rw));
    if(contains(mousePos))
    {
        eventHandler(rw, handle_owner);
    } else if (handle_owner->getCentre() == centre)  {
        eventHandler(rw, handle_owner);
    }

}

void BaseRectangleButton::setColor(const sf::Color& color_)
{
    rect.setFillColor(color_);
}

void BaseRectangleButton::on_release()
{
    std::cout << "pressed" << std::endl;
}


void BaseRectangleButton::eventHandler(sf::RenderWindow& rw, Element*& handle_owner)
{
    sf::Vector2f mousePos = vector_i_to_f(sf::Mouse::getPosition(rw));
    bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    OwnershipHandler(handle_owner);
    // If mouse pressed, mouse inside button and button not already pressed, press
    if(pressed && state != PRESSED && contains(mousePos))
    {
        state = PRESSED;
        return;
    }
    // If its not pressed but state is pressed unpress
    else if (!pressed && state == PRESSED)
    {
        if(contains(mousePos))
            state = HOVER;
        else
            state = RELAX;
        on_release();
        return;
    }
    if(state != PRESSED)
    {
        if(contains(mousePos))
            state = HOVER;
        else
        {
            state = RELAX;
            handle_owner = nullptr;
        }
    }
}

void BaseRectangleButton::update()
{
    if(state == RELAX)
        setColor(style.button_style.rest_color);
    else if(state == HOVER)
        setColor(style.button_style.hover_color);
    else if(state == PRESSED)
        setColor(style.button_style.pressed_color);
}

void BaseRectangleButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(rect);
    target.draw(style.text_style.getTextRef());
}


TextBoxStyle::TextBoxStyle(const TextStyle& text_style_, const sf::Color& back_drop_color_)
    : text_style(text_style_), back_drop_color(back_drop_color_) {}

TextBoxStyle::TextBoxStyle() : back_drop_color(sf::Color::Green) {}



TextBox::TextBox(const TextBoxStyle& style_, const Rectangle& bounding_box_) : style(style_), 
    bounding_box(bounding_box_), centre(getBoxCentre(bounding_box))
{
    setRectSize(back_drop, bounding_box);
    setRectOrigin(back_drop);
    back_drop.setFillColor(style.back_drop_color);
    back_drop.setPosition(centre);
    style.text_style.setTextPosition(centre);
}

void TextBox::setPosition(const sf::Vector2f& position_)
{
    back_drop.setPosition(position_);
    style.text_style.setTextPosition(position_);
}


TextBox::TextBox(const TextBoxStyle& style_, const Rectangle& parent_box, const float& proportion)
    : style(style_)
{
    assert(proportion > 0 && proportion <= 1);
    float box_height = parent_box.getHeight();
    float box_width = parent_box.getWidth();
    style.text_style.setTextSize(box_height * proportion);
    if(style.text_style.getTextRef().getLocalBounds().width > box_width)
    {
        while(style.text_style.getTextRef().getLocalBounds().width > box_width)
            style.text_style.setTextSize(style.text_style.getTextSize() - 1);
    }
    style.text_style.centre();
    bounding_box = Rectangle(parent_box.topLeft(), Vect2f(parent_box.topLeft()) + Vect2f(
                style.text_style.getTextRef().getLocalBounds().width, box_height * proportion));
    setRectSize(back_drop, bounding_box);
    setRectOrigin(back_drop);
    style.text_style.setTextPosition(centre);
    back_drop.setFillColor(style.back_drop_color);
    centre = getBoxCentre(bounding_box);
    setPosition(centre);
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(back_drop);
    target.draw(style.text_style.getTextRef());
}


SliderStyle::SliderStyle(const ButtonStyle& progress_circle_style_, 
        const sf::Color& bar_color_, const sf::Color& end_circle_color_)
    : progress_circle_style(progress_circle_style_), bar_color(bar_color_), end_circle_color(end_circle_color_) {}

SliderStyle::SliderStyle() : end_circle_color(66, 118, 108), bar_color(165, 1, 4) {}

sf::Vector2f TextBox::getCentre() const 
{
    return centre;
}

float distance(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
{
    return std::sqrt( std::pow((lhs.x - rhs.x),2)  + std::pow((lhs.y - rhs.y),2));
}

std::string roundFloatString(const float& val)
{
    std::ostringstream ss;
    ss << val << std::setprecision(2);
    return std::string(ss.str());
}


Slider::Slider(const Rectangle& bounding_box_, const SliderStyle& style_, const float min, const float& max,
            TextBoxStyle title_style) : Element(bounding_box_), progress_circle_style(style.progress_circle_style),
            style(style_), max_value(max), min_value(min)

{
    sharedConstructorFacilities(style_, min, max, title_style);
}

void Slider::sharedConstructorFacilities(const SliderStyle& style_, const float min, const float& max,
            TextBoxStyle title_style)
{
    title = std::make_unique<TextBox>(title_style, bounding_box, 2.0f/5.0f);
    // Set up the end indicators
    float end_indicator_height = bounding_box.getHeight() / 5.0f;
    min_value_text = std::make_unique<TextStyle>(end_indicator_height, title_style.text_style.getTextColor(),
            title_style.text_style.getTextFont(), roundFloatString(min));
    max_value_text = std::make_unique<TextStyle>(*min_value_text);
    max_value_text->setTextString(roundFloatString(max));
    max_value_text->setTextString(max_value_text->getTextString());
    float min_max_y = bounding_box.y_max - (end_indicator_height / 2);
    float min_max_x_buffer = max_value_text->getTextRef().getLocalBounds().width / 2;

    // Set up the actual Slider
    float circle_radius =  end_indicator_height;
    float bar_y = bounding_box.y_max - (circle_radius * 2.0f);
    float x_centre = centre.x;
    // Set up sizes
    right_side_circle.setRadius(circle_radius);
    left_side_circle.setRadius(circle_radius);
    progress_circle.setRadius(circle_radius);
    bar.setSize(sf::Vector2f(bounding_box.getWidth(), (circle_radius * 2.0f)));
    // Set origins
    setCircleOrigin(right_side_circle);
    setCircleOrigin(left_side_circle);
    setCircleOrigin(progress_circle);
    setRectOrigin(bar);
    // Set Positions 
    left_side_circle.setPosition(bounding_box.x_min + circle_radius, bar_y);
    right_side_circle.setPosition(bounding_box.x_max - circle_radius, bar_y);
    progress_circle.setPosition(x_centre, bar_y);
    bar.setPosition(x_centre, bar_y);
    max_value_text->setTextPosition(sf::Vector2f(right_side_circle.getPosition().x, min_max_y));
    min_value_text->setTextPosition(sf::Vector2f(left_side_circle.getPosition().x, min_max_y));
    
    // Set Colors
    left_side_circle.setFillColor(progress_circle_style.rest_color);
    right_side_circle.setFillColor(progress_circle_style.hover_color);
    progress_circle.setFillColor(progress_circle_style.rest_color);
    bar.setFillColor(style.bar_color);
    // Set value
    value = (max + min) / 2;
    // set up progress_bar
    progress_bar.setSize(sf::Vector2f(bounding_box.getWidth() / 2.0f - circle_radius, circle_radius));
    progress_bar.setOrigin(0, circle_radius / 2.0f);
    progress_bar.setPosition(left_side_circle.getPosition());
    progress_bar.setFillColor(progress_circle_style.rest_color);
    // Value tracking stuff
    range = max_value - min_value;
    width = right_side_circle.getPosition().x - left_side_circle.getPosition().x;


}

bool Slider::inProgressCircle(const sf::Vector2f& point)
{
    return distance(point, progress_circle.getPosition()) <= progress_circle.getRadius();
}





void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(bar);
    target.draw(progress_bar);
    target.draw(left_side_circle);
    target.draw(right_side_circle);
    target.draw(progress_circle);
    target.draw(*title);
    target.draw(*min_value_text);
    target.draw(*max_value_text);
}

void Slider::claim_handle(sf::RenderWindow &rw, Element *&handle_owner)
{
    sf::Vector2f mousePos = vector_i_to_f(sf::Mouse::getPosition(rw));
    if(inProgressCircle(mousePos))
    {
        eventHandler(rw, handle_owner);
    } else if (handle_owner->getCentre() == centre)  {
        eventHandler(rw, handle_owner);
    }
}



void Slider::eventHandler(sf::RenderWindow& rw, Element *&handle_owner)
{
    OwnershipHandler(handle_owner);
    bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    sf::Vector2f mousePos = vector_i_to_f(sf::Mouse::getPosition(rw));
    if(pressed && state != PRESSED && inProgressCircle(mousePos))
    {
        state = PRESSED;
        return;
    }
    // If its not pressed but state is pressed unpress
    if (!pressed && state == PRESSED)
    {
        if(inProgressCircle(mousePos))
            state = HOVER;
        else
            state = RELAX;
        return;
    }
    if(state != PRESSED)
    {
        if(inProgressCircle(mousePos))
            state = HOVER;
        else
        {
            state = RELAX;
            handle_owner = nullptr;
        }
    }

    if(state == PRESSED)
    {
        if(mousePos.x < right_side_circle.getPosition().x && mousePos.x > left_side_circle.getPosition().x)
            setCircleX(progress_circle, mousePos.x);
        else if(mousePos.x  > centre.x)
            setCircleX(progress_circle, right_side_circle.getPosition().x);
        else
            setCircleX(progress_circle, left_side_circle.getPosition().x);
        value = range * (progress_circle.getPosition().x / width);
    }

}

void Slider::update()
{
   progress_bar.setSize(sf::Vector2f(progress_circle.getPosition().x - left_side_circle.getPosition().x, 
               progress_bar.getSize().y)); 
    if(state == RELAX)
        progress_circle.setFillColor(style.progress_circle_style.rest_color);
    else if(state == HOVER)
        progress_circle.setFillColor(style.progress_circle_style.hover_color);
    else if(state == PRESSED)
        progress_circle.setFillColor(style.progress_circle_style.pressed_color);
    progress_bar.setFillColor(progress_circle.getFillColor());
   
}

Slider::Slider(const Slider& other) : Element(other.bounding_box)
{
    value = other.value;
    range = other.range;
    width = other.width;
    max_value = other.max_value;
    min_value = other.min_value;

    min_value_text = std::make_unique<TextStyle>(*other.min_value_text);
    max_value_text = std::make_unique<TextStyle>(*other.max_value_text);
    style = other.style;
    title = std::make_unique<TextBox>(*other.title);
    progress_circle_style = other.progress_circle_style;
    left_side_circle = other.left_side_circle;
    right_side_circle = other.right_side_circle;
    progress_bar = other.progress_bar;
    progress_circle = other.progress_circle;
    bar = other.bar;
}


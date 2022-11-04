#include <element.hpp>
#include <geometery.hpp>
#include <utility>
#include <cmath>
#include <exception>
#include <numeric>
#include <vector>
#include <initializer_list>
#include <concepts>
#include <SFML/Graphics.hpp>

#include <iostream>



constexpr size_t factorial(const size_t& n) {
   if ((n==0)||(n==1))
       return 1;
   else
    return n*factorial(n-1);
}

constexpr size_t combinatoric(const size_t& n, const size_t& r)
{
    return factorial(n)/(factorial(n-r) * factorial(r));
};


template<typename T, size_t N>
constexpr std::array<std::pair<T, T>, combinatoric(N, 2)> get_pairs(const std::array<T, N>& set)
{
    if (set.size() == 1)
        throw std::runtime_error("not a proper array");
    else
    {
        std::array<std::pair<size_t, size_t>, combinatoric(N, 2)> index_pairs;
        // Create bit set
        std::array<bool, N> bitset;
        for(size_t i = 0; i < N; i++)
        {
            if(i < 2)
                bitset[i] = 1;
            else
                bitset[i] = 0;
        }

        //
        for(auto& pair : index_pairs)
        {
            size_t first;
            bool found = false;
            for(size_t i = 0; i < N; i++)
            {
                if(bitset[i])
                {
                    if(not found)
                    {
                        found = true;
                        first = i;
                    } 
                    else if (found)
                    {
                        pair = std::make_pair(first, i);
                        break;
                    }
                }
            }
            found = false;
            std::prev_permutation(bitset.begin(), bitset.end());
        }
        std::array<std::pair<T, T>, combinatoric(N, 2)> final_pairs;
        auto trans_lambda = [&set](const std::pair<size_t, size_t>& c){return std::make_pair(set[c.first], set[c.second]);};
        std::transform(index_pairs.begin(), index_pairs.end(), final_pairs.begin(), trans_lambda);
        return final_pairs;
    }
}

template<typename T>
constexpr std::array<std::pair<T, T>, 1> get_pairs(const std::array<T, 1>& set)
{
    return std::array<std::pair<T,T>, 1>{std::make_pair(set[0], set[1])};
}

// Returns false if there is intersection between boxes
template<size_t N>
constexpr bool box_check(const std::array<Rectangle, N>& bound_boxes)
{
    if(bound_boxes.size() == 1)
        return true;
    else
    {
        auto pairs = get_pairs(bound_boxes);
        for(const auto& i : pairs)
            if(intersectionChecker(i.first, i.second))
                return false;
        return true;
    }
}

template<Rectangle ... BoundBoxes>
struct BoxChecker
{
    static constexpr bool value = box_check(std::array<Rectangle, sizeof...(BoundBoxes)>{BoundBoxes...});
};

template<typename T, typename std::enable_if<T::value, int> = 0>
class Validator;


struct WindowSize
{
    uint width, height;
};



class BackGround : public Element
{
public:
    BackGround(const sf::RenderWindow& window);
    void claim_handle(sf::RenderWindow& rw, Element*& handle_owner);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void eventHandler(sf::RenderWindow& rw, Element*& handle_owner);
};

template<WindowSize Size, Rectangle ... BoundBoxes>
requires BoxChecker<BoundBoxes...>::value
class Window
{
public:


    template<typename ... T>
    Window(T ... t) : rWindow(sf::VideoMode(1920, 1080), "Window"), back_ground(rWindow)
    {
        elements.reserve(sizeof...(t));
        fillVector(t...);
    }
    
    void loop()
    {
        Element *current = &back_ground;
        while (rWindow.isOpen())
        {
            sf::Event event;
                while(rWindow.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        rWindow.close();
                }
            rWindow.clear();
            for(auto& element : elements)
            {
                element->claim_handle(rWindow, current);
                if(current == nullptr)
                    current = &back_ground;
            }
            for(auto& element : elements)
            {
                element->update();
                rWindow.draw(*element.get());
            }
            rWindow.display();
        }
    }
    
private:
    std::vector<std::unique_ptr<Element>> elements;
    sf::RenderWindow rWindow;
    sf::Color backGroundColor = sf::Color::White;
    sf::Vector2i windowSize;
    BackGround back_ground;
    template<typename T>
    requires std::derived_from<T, Element>
    void fillVector(T t)
    {
        elements.push_back(std::make_unique<T>(t));
    }

    template<typename  T, typename ... T2>
    void fillVector(T t, T2 ... t2)
    {
       fillVector(t);
       fillVector(t2 ...);
    }


};

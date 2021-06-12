
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include "Car.h"

using namespace std::chrono_literals;  // ns, us, ms, s, h, etc.
using namespace std;
const int H = 1000, W = 1000;

int main() {
    sf::RenderWindow window(sf::VideoMode(H, W), "Car simulator");
    Car car = Car(200, 200);
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/arial.ttf");
    sf::Text text;
    // select the font
    text.setFont(font); // font is a sf::Font
    // set the string to display
    text.setString("Hello world");
    // set the character size
    text.setCharacterSize(24); // in pixels, not points!
    // set the color
    text.setFillColor(sf::Color::Red);
    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);


    while (window.isOpen()) {
        sf::Event event;
        car.throttle = 0.0;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                 event.key.code == sf::Keyboard::Escape))
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            car.turn(-0.1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            car.turn(0.1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            car.throttle = -1.0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            car.throttle = 1.0;
        }

        window.clear();
        car.update();
        car.draw(window);
        text.setString(to_string(car.getSpeed()));
        window.draw(text);
        window.display();
        this_thread::sleep_for(10ms);
    }

    return 0;
}
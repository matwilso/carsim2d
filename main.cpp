
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

//#include "Car.h"
#include "Simulator.h"
#include "Constants.h"

using namespace std::chrono_literals;  // ns, us, ms, s, h, etc.
using namespace std;
const int H = 1000, W = 1000;

int main() {
    sf::RenderWindow window(sf::VideoMode(H, W), "Car simulator");
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/arial.ttf");
    sf::Text text;
    text.setFont(font);
    text.setString("Hello world");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    Simulator sim;
    int controlState = 0;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                 event.key.code == sf::Keyboard::Escape))
                window.close();
        }
        //car.throttle = 0.0;
        controlState = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            controlState |= TDC_LEFT;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            controlState |= TDC_RIGHT;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            controlState |= TDC_UP;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            controlState |= TDC_DOWN;
        }
        window.clear();
        //window.clear(sf::Color::White);
        sim.step(controlState, &window);
        //car.update();
        //car.draw(window);
        //text.setString(to_string(car.getSpeed()));
        window.draw(text);
        window.display();
        this_thread::sleep_for(10ms);
    }
    return 0;
}
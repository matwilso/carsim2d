
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

//#include "Car.h"
#include "Common.h"
#include "Simulator.h"

using namespace std::chrono_literals;  // ns, us, ms, s, h, etc.
using namespace std;
const int H = 720, W = 1080;

int main() {
    sf::RenderWindow window(sf::VideoMode(W, H), "Car simulator");
    Simulator sim;
    int controlState = 0;
    // run main simulator/render loop
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                 event.key.code == sf::Keyboard::Escape))
                window.close();
        }
        controlState = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            controlState |= TDC_LEFT;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            controlState |= TDC_RIGHT;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            controlState |= TDC_UP;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            controlState |= TDC_DOWN;
        window.clear(sf::Color(128, 128, 128));
        sim.step(controlState, &window);

        window.display();
        this_thread::sleep_for(10ms);
    }
    return 0;
}


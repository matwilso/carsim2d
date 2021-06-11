
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>

#include "Car.h"

using namespace std::chrono_literals;  // ns, us, ms, s, h, etc.
using namespace std;
const int H = 800, W = 800;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
    Car car = Car(200, 200);

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
        window.display();
        this_thread::sleep_for(10ms);
    }

    return 0;
}
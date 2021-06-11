#include <SFML/Graphics.hpp>
#include <iostream>
#include "Car.h"

using namespace std;
const int H = 800, W = 800;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
    Car car = Car(200, 200);

    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2f delta(0.0, 0.0);

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                 event.key.code == sf::Keyboard::Escape))
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                        car.up();
                        delta = sf::Vector2f(0.0, 10.0);
                        cout << "up" << endl;
                        break;
                    case sf::Keyboard::Down:
                        delta = sf::Vector2f(0.0, -10.0);
                        cout << "down" << endl;
                        break;
                    default:
                        car.noop();
                        break;
                }
            }
        }

        window.clear();
        car.update();
        car.draw(window);
        window.display();
    }

    return 0;
}
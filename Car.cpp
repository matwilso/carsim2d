#include "Car.h"

Car::Car(int x = 400, int y = 400) {
    body.setPosition(400, 400);
    body.setFillColor(sf::Color::Green);
    body.setRotation(33);
}
void Car::update() { body.setPosition(body.getPosition()); }

void Car::draw(sf::RenderWindow& window) {
    for (auto& shape : shapes) {
        window.draw(*shape);
    }
}

void Car::up() { engine.x = 0, engine.y = 0; }
void Car::noop() { engine.x = 0, engine.y = 0; }

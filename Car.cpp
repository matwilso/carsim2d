#include "Car.h"
#include <algorithm>
#include <iostream>
#include <math.h>

double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

Car::Car(int x = 400, int y = 400) {
    body.setPosition(400, 400);
    body.setOrigin(25.0f, 50.0f);
    body.setFillColor(sf::Color::Green);
    for (auto& wheel : wheels) {
        wheel->setFillColor(sf::Color::Red);
        wheel->setFillColor(sf::Color::Red);
        wheel->setFillColor(sf::Color::Red);
        wheel->setFillColor(sf::Color::Red);
        wheel->setOrigin(5.0f, 10.0f);
    }
}
void Car::update() {
    //std::cout << "angle " << wheelAngle << " sin " << sin(wheelAngle) << std::endl;
    sf::Vector2f delta = throttle * sf::Vector2f(-sin(deg2rad(wheelAngle)), cos(deg2rad(wheelAngle)));
    body.setPosition(body.getPosition() + delta);

    wheel1.setRotation(wheelAngle);
    wheel1.setPosition(body.getPosition() + sf::Vector2f(-20, -30));

    wheel2.setRotation(wheelAngle);
    wheel2.setPosition(body.getPosition() + sf::Vector2f(20, -30));

    wheel3.setPosition(body.getPosition() + sf::Vector2f(-20, 30));

    wheel4.setPosition(body.getPosition() + sf::Vector2f(20, 30));
}

void Car::draw(sf::RenderWindow& window) {
    for (auto& shape : shapes) {
        window.draw(*shape);
    }
}

void Car::turn(float dir) {
    wheelAngle = std::clamp<float>(wheelAngle + 10 * dir, -50.0, 50.0);
};
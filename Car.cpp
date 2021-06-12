#include "Car.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <armadillo>
#include "include/box2d/box2d.h"


using namespace std;

double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

sf::Vector2f operator*(const sf::Vector2f& vec, const sf::Vector2f& v2) {
    return sf::Vector2f(vec.x*v2.x, vec.y*v2.y);
}
sf::Vector2f operator/(const sf::Vector2f& vec, const sf::Vector2f& v2) {
    return sf::Vector2f(vec.x/v2.x, vec.y/v2.y);
}
sf::Vector2f operator/(const sf::Vector2f& vec, float f) {
    return sf::Vector2f(vec.x/f, vec.y/f);
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

float sum(sf::Vector2f vec) {
    return vec.x+vec.y;
}

float mag(sf::Vector2f vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y);
}
sf::Vector2f norm(sf::Vector2f vec) {
    return vec / mag(vec);
}


sf::Vector2f Car::drag() {
    return -0.1f * mag(vel) * vel;
}
float Car::getSpeed() {
    return mag(vel);
}

float component(sf::Vector2f vec) {

}

void Car::update() {
    //std::cout << "angle " << wheelAngle << " sin " << sin(wheelAngle) << std::endl;
    dir.x = -sin(deg2rad(wheelAngle));
    dir.y = cos(deg2rad(wheelAngle));

    //sf::Vector2f delta = throttle * sf::Vector2f(-sin(deg2rad(wheelAngle)), cos(deg2rad(wheelAngle)));
    acc = throttle*dir + drag();
    vel += acc * dt ;
    vel *= linearDamping;


    float R = 60 / sin(wheelAngle);
    angVel = -30*mag(vel) / R;

    body.setPosition(body.getPosition() + vel);
    body.setRotation(body.getRotation() + angVel);

    wheel1.setRotation(body.getRotation() + wheelAngle);
    wheel1.setPosition(body.getPosition() + sf::Vector2f(-20, -30));

    wheel2.setRotation(body.getRotation() + wheelAngle);
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
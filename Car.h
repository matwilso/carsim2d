#pragma once
#include <SFML/Graphics.hpp>

class Car {
   public:
    Car(int, int);
    void update();
    void draw(sf::RenderWindow&);
    void turn(float angle);
    float getSpeed();
    float throttle = 0.0;
    sf::Vector2f acc = sf::Vector2f(0.0, 0.0);
    sf::Vector2f vel = sf::Vector2f(0.0, 0.0);
    float angVel = 0.0;
    sf::Vector2f dir = sf::Vector2f(0.0, 1.0);
    float wheelAngle = 0.0;
    float dt = 0.1;

   private:
    sf::Vector2f drag();
    float wheelRadius = 1.0;
    float linearDamping = 0.99;
    sf::RectangleShape body = sf::RectangleShape(sf::Vector2f(50.f, 100.f));
    sf::RectangleShape wheel1 = sf::RectangleShape(sf::Vector2f(10.f, 20.0f));
    sf::RectangleShape wheel2 = sf::RectangleShape(sf::Vector2f(10.f, 20.0f));
    sf::RectangleShape wheel3 = sf::RectangleShape(sf::Vector2f(10.f, 20.0f));
    sf::RectangleShape wheel4 = sf::RectangleShape(sf::Vector2f(10.f, 20.0f));
    std::vector<sf::Shape*> shapes = {&body, &wheel1, &wheel2, &wheel3, &wheel4};
    std::vector<sf::Shape*> wheels = {&wheel1, &wheel2, &wheel3, &wheel4};
};

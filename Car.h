#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Car {
   public:
    Car(int, int);
    void update();
    void draw(sf::RenderWindow&);
    void up();
    sf::Vector2f acc = sf::Vector2f(0.0, 0.0);
    double accel = 0.0;
    sf::Vector2f vel = sf::Vector2f(0.0, 0.0);
    sf::Vector2f dir = sf::Vector2f(0.0, 0.0);
    double wheelAngle = 0.0;

   private:
    double wheelRadius = 1.0;
    sf::RectangleShape body = sf::RectangleShape(sf::Vector2f(50.f, 100.f));
    std::vector<sf::Shape*> shapes = {&body};
};

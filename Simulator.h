#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "Car.h"
#include "include/box2d/box2d.h"

class Simulator {
   public:
    Simulator();
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();
    void applyDrive(int);
    void applyTurn(int);
    void applyFriction();

    ~Simulator();
    void step(int controlState, sf::RenderWindow&);
    b2World* world;
    float dt = 0.01;
    Car* car;
    //unique_ptr<Car> car;
};

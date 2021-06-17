#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

#include "Camera.h"
#include "Car.h"
#include "include/box2d/box2d.h"

class Simulator {
   public:
    Simulator();
    ~Simulator();
    void step(int controlState, sf::RenderWindow*);

    float dt = 0.01;
    b2World* world;
    Car* car;
    Camera* frontCam;
    b2Body* edge;
};

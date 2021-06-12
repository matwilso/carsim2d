#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "include/box2d/box2d.h"
enum {
    TDC_LEFT = 0x1,
    TDC_RIGHT = 0x2,
    TDC_UP = 0x4,
    TDC_DOWN = 0x8
};


class Simulator {
   public:
    Simulator();
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();
    void updateDrive(int);
    void updateTurn(int);
    void updateFriction();

    ~Simulator();
    void step(int controlState, sf::RenderWindow&);
    b2World* world;

    b2Body* wheel;
    float dt = 0.01;
    //private:
};

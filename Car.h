#pragma once
#include <math.h>

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>

#include "Wheel.h"
#include "include/box2d/box2d.h"
using namespace std;

class Car {
    b2RevoluteJoint *flJoint, *frJoint;

   public:
    b2Body* body;
    std::vector<Wheel*> wheels;
    Car(b2World* world);
    void update(int controlState);
    ~Car();
};
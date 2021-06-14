#include "Car.h"

#include <math.h>

#include <algorithm>
#include <iostream>

#include "Common.h"
#include "include/box2d/box2d.h"
using namespace std;

Car::Car(b2World* world) {
    //create car body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(10, -150);
    body = world->CreateBody(&bodyDef);
    body->SetAngularDamping(3);

    b2Vec2 vertices[6];
    vertices[0].Set(2, 0);
    vertices[1].Set(2, 2.5);
    vertices[2].Set(1.5, 10);
    vertices[3].Set(-1.5, 10);
    vertices[4].Set(-2, 2.5);
    vertices[5].Set(-2, 0);
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices, 6);

    b2FixtureDef fixDef;
    fixDef.shape = &polygonShape;
    fixDef.density = 0.1f;
    //fixDef.userData = FixtureUserData(sf::Color::Black);
    auto ud = new FixtureUserData;
    ud->color = sf::Color::Red;
    fixDef.userData.pointer = reinterpret_cast<uintptr_t>(ud);
    //b2Fixture* fixture = body->CreateFixture(&fixDef);  //shape, density

    b2Fixture* fixture = body->CreateFixture(&fixDef);  //shape, density
    //b2Fixture* fixture = body->CreateFixture(&polygonShape, 0.1f);  //shape, density

    //prepare common joint parameters
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = body;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorB.SetZero();  //center of tire

    float maxForwardSpeed = 250;
    float maxBackwardSpeed = -100;
    float backTireMaxDriveForce = 300;
    float frontTireMaxDriveForce = 500;
    float backTireMaxLateralImpulse = 8.5;
    float frontTireMaxLateralImpulse = 7.5;

    //back left tire
    Wheel* tire = new Wheel(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
    jointDef.bodyB = tire->body;
    jointDef.localAnchorA.Set(-3, 0.75f);
    world->CreateJoint(&jointDef);
    wheels.push_back(tire);

    //back right tire
    tire = new Wheel(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
    jointDef.bodyB = tire->body;
    jointDef.localAnchorA.Set(3, 0.75f);
    world->CreateJoint(&jointDef);
    wheels.push_back(tire);

    //front left tire
    tire = new Wheel(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
    jointDef.bodyB = tire->body;
    jointDef.localAnchorA.Set(-3, 8.5f);
    flJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
    wheels.push_back(tire);

    //front right tire
    tire = new Wheel(world);
    tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
    jointDef.bodyB = tire->body;
    jointDef.localAnchorA.Set(3, 8.5f);
    frJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
    wheels.push_back(tire);
}

Car::~Car() {
    body->GetWorld()->DestroyBody(body);
    for (int i = 0; i < wheels.size(); i++)
        delete wheels[i];
}

void Car::update(int controlState) {
    for (int i = 0; i < wheels.size(); i++)
        wheels[i]->updateFriction();
    for (int i = 0; i < wheels.size(); i++)
        wheels[i]->updateDrive(controlState);

    //control steering
    float lockAngle = 35 * DEGTORAD;
    float turnSpeedPerSec = 160 * DEGTORAD;  //from lock to lock in 0.5 sec
    float turnPerTimeStep = turnSpeedPerSec / 60.0f;
    float desiredAngle = 0;
    switch (controlState & (TDC_LEFT | TDC_RIGHT)) {
        case TDC_LEFT:
            desiredAngle = lockAngle;
            break;
        case TDC_RIGHT:
            desiredAngle = -lockAngle;
            break;
        default:;  //nothing
    }
    float angleNow = flJoint->GetJointAngle();
    float angleToTurn = desiredAngle - angleNow;
    angleToTurn = b2Clamp(angleToTurn, -turnPerTimeStep, turnPerTimeStep);
    float newAngle = angleNow + angleToTurn;
    flJoint->SetLimits(newAngle, newAngle);
    frJoint->SetLimits(newAngle, newAngle);
}

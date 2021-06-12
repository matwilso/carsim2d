#include "Simulator.h"

#include <math.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>

#include "include/box2d/box2d.h"

using namespace std;

Simulator::Simulator() {
    b2Vec2 gravity(0.0f, 0.0f);
    world = new b2World(gravity);

    b2BodyDef wheelDef;
    wheelDef.type = b2_dynamicBody;
    wheel = world->CreateBody(&wheelDef);

    b2PolygonShape polyShape;
    polyShape.SetAsBox(0.5f, 1.25f);
    wheel->CreateFixture(&polyShape, 1);
}

b2Vec2 Simulator::getLateralVelocity() {
    b2Vec2 currentRightNormal = wheel->GetWorldVector(b2Vec2(1, 0));
    return b2Dot(currentRightNormal, wheel->GetLinearVelocity()) * currentRightNormal;
}

void Simulator::updateFriction() {
    //lateral linear velocity
    float maxLateralImpulse = 2.5f;
    b2Vec2 impulse = wheel->GetMass() * -getLateralVelocity();
    if (impulse.Length() > maxLateralImpulse)
        impulse *= maxLateralImpulse / impulse.Length();
    wheel->ApplyLinearImpulse(impulse, wheel->GetWorldCenter(), 1);

    //angular velocity
    wheel->ApplyAngularImpulse(0.1f * wheel->GetInertia() * -wheel->GetAngularVelocity(), 1);

    //forward linear velocity
    b2Vec2 currentForwardNormal = getForwardVelocity();
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragForceMagnitude = -2 * currentForwardSpeed;
    wheel->ApplyForce(dragForceMagnitude * currentForwardNormal, wheel->GetWorldCenter(), 1);
}

b2Vec2 Simulator::getForwardVelocity() {
    b2Vec2 currentForwardNormal = wheel->GetWorldVector(b2Vec2(0, 1));
    return b2Dot(currentForwardNormal, wheel->GetLinearVelocity()) * currentForwardNormal;
}

void Simulator::updateDrive(int controlState) {
    //find desired speed
    float desiredSpeed = 0;
    switch (controlState & (TDC_UP | TDC_DOWN)) {
        case TDC_UP:
            desiredSpeed = 100;
            break;
        case TDC_DOWN:
            desiredSpeed = -20;
            break;
        default:
            return;  //do nothing
    }

    //find current speed in forward direction
    b2Vec2 currentForwardNormal = wheel->GetWorldVector(b2Vec2(0, 1));
    float currentSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);

    //apply necessary force
    float force = 0;
    if (desiredSpeed > currentSpeed)
        force = 150;
    else if (desiredSpeed < currentSpeed)
        force = -150;
    else
        return;
    wheel->ApplyForce(force * currentForwardNormal, wheel->GetWorldCenter(), 1);
}

void Simulator::updateTurn(int controlState) {
    float desiredTorque = 0;
    switch (controlState & (TDC_LEFT | TDC_RIGHT)) {
        case TDC_LEFT:
            desiredTorque = 15;
            break;
        case TDC_RIGHT:
            desiredTorque = -15;
            break;
        default:;  //nothing
    }
    wheel->ApplyTorque(desiredTorque, 1);
}

void Simulator::step(int controlState, sf::RenderWindow& window) {
    updateFriction();
    updateDrive(controlState);
    updateTurn(controlState);
    world->Step(dt, 10, 10);
    cout << "pos " << wheel->GetPosition().x << endl;
    auto fix = wheel->GetFixtureList()[0];
    if (auto shape = dynamic_cast<b2PolygonShape*>(fix.GetShape())) {
        auto trans = fix.GetBody()->GetTransform();
        sf::ConvexShape convex;
        int n = shape->m_count;
        convex.setPointCount(n);
        for (auto i = 0; i < n; i++) {
            auto vert = wheel->GetWorldPoint(shape->m_vertices[i]);
            sf::Vector2f arr(100 + 10.0 * vert.x, 100 - 10.0 * vert.y);
            cout << "i " << i << " " << arr.x << " " << arr.y << endl;
            convex.setPoint(i, arr);
        }
        convex.setFillColor(sf::Color(100, 250, 50));
        window.draw(convex);
    }
    //else if (auto shape = dynamic_cast<b2PolygonShape*>(fix.GetShape())) { }
}

Simulator::~Simulator() {
    wheel->GetWorld()->DestroyBody(wheel);
}
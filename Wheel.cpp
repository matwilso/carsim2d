#include "Wheel.h"

#include <math.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>

#include "Common.h"
#include "Simulator.h"
#include "include/box2d/box2d.h"

using namespace std;

Wheel::Wheel(b2World* world) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(10, -150);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.5f, 1.25f);
    b2FixtureDef fixDef;
    fixDef.shape = &polygonShape;
    fixDef.density = 1.0f;
    auto ud = new FixtureUserData;
    ud->color = sf::Color::Black;
    fixDef.userData.pointer = reinterpret_cast<uintptr_t>(ud);
    b2Fixture* fixture = body->CreateFixture(&fixDef);  //shape, density
    m_currentTraction = 1;
}

Wheel::~Wheel() {
    body->GetWorld()->DestroyBody(body);
}

void Wheel::setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse) {
    m_maxForwardSpeed = maxForwardSpeed;
    m_maxBackwardSpeed = maxBackwardSpeed;
    m_maxDriveForce = maxDriveForce;
    m_maxLateralImpulse = maxLateralImpulse;
}

b2Vec2 Wheel::getLateralVelocity() {
    b2Vec2 currentRightNormal = body->GetWorldVector(b2Vec2(1, 0));
    return b2Dot(currentRightNormal, body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 Wheel::getForwardVelocity() {
    b2Vec2 currentForwardNormal = body->GetWorldVector(b2Vec2(0, 1));
    return b2Dot(currentForwardNormal, body->GetLinearVelocity()) * currentForwardNormal;
}

void Wheel::updateFriction() {
    //lateral linear velocity
    b2Vec2 impulse = body->GetMass() * -getLateralVelocity();
    if (impulse.Length() > m_maxLateralImpulse)
        impulse *= m_maxLateralImpulse / impulse.Length();
    body->ApplyLinearImpulse(m_currentTraction * impulse, body->GetWorldCenter(), 1);

    //angular velocity
    body->ApplyAngularImpulse(m_currentTraction * 0.1f * body->GetInertia() * -body->GetAngularVelocity(), 1);

    //forward linear velocity
    b2Vec2 currentForwardNormal = getForwardVelocity();
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragForceMagnitude = -2 * currentForwardSpeed;
    body->ApplyForce(m_currentTraction * dragForceMagnitude * currentForwardNormal, body->GetWorldCenter(), 1);
}

void Wheel::updateDrive(int controlState) {
    //find desired speed
    float desiredSpeed = 0;
    switch (controlState & (TDC_UP | TDC_DOWN)) {
        case TDC_UP:
            desiredSpeed = m_maxForwardSpeed;
            break;
        case TDC_DOWN:
            desiredSpeed = m_maxBackwardSpeed;
            break;
        default:
            return;  //do nothing
    }

    //find current speed in forward direction
    b2Vec2 currentForwardNormal = body->GetWorldVector(b2Vec2(0, 1));
    float currentSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);

    //apply necessary force
    float force = 0;
    if (desiredSpeed > currentSpeed)
        force = m_maxDriveForce;
    else if (desiredSpeed < currentSpeed)
        force = -m_maxDriveForce;
    else
        return;
    body->ApplyForce(m_currentTraction * force * currentForwardNormal, body->GetWorldCenter(), 1);
}

void Wheel::updateTurn(int controlState) {
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
    body->ApplyTorque(desiredTorque, 1);
}
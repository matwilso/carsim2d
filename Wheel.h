#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "include/box2d/box2d.h"
class Wheel {
   public:
    Wheel(b2World*);
    ~Wheel();
    void setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse);
    //void addGroundArea(GroundAreaFUD* ga);
    //void removeGroundArea(GroundAreaFUD* ga);
    void updateTraction();
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();
    void updateFriction();
    void updateDrive(int controlState);
    void updateTurn(int controlState);

    b2Body* body;
    float m_maxForwardSpeed;
    float m_maxBackwardSpeed;
    float m_maxDriveForce;
    float m_maxLateralImpulse;
    //std::set<GroundAreaFUD*> m_groundAreas;
    float m_currentTraction;
};
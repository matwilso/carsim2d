#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Camera.h"
#include "include/box2d/box2d.h"

class Camera {
   public:
    Camera(std::string, b2Body*, b2Vec2, float);
    b2Body* body;
    b2Vec2 offset;
    std::string name;
    float angle;
    float fov = 1.5707;
    //float fov = 0.872;
    float range = 100;
    int samples = 64;
    int upsize = 3;
    int imageWidth = samples * upsize;
    int imageHeight = imageWidth / 2;
    sf::Texture rgbTexture;
    sf::Uint8* rgbPixels;
    sf::Texture capture(sf::RenderWindow*);
};

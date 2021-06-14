#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

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
    sf::Texture depthTexture;
    sf::Uint8* rgbPixels;
    sf::Uint8* depthPixels;
    std::pair<sf::Texture, sf::Texture> capture(sf::RenderWindow*);
    int totalSize();
    sf::Uint8 buffer[64];
};

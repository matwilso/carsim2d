#pragma once
#include <SFML/Graphics.hpp>
#include <utility>

#include "Camera.h"
#include "include/box2d/box2d.h"

class Camera {
   public:
    Camera();
    float fov = 90;
    float range = 10;
    int imageWidth = 128;
    int imageHeight = 128;
    sf::Texture rgbTexture;
    sf::Texture depthTexture;
    sf::Uint8* rgbPixels;
    sf::Uint8* depthPixels;
    std::pair<sf::Texture, sf::Texture> capture();
    int totalSize();
};

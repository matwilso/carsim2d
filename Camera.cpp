
#include "Camera.h"

#include <math.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <utility>

using namespace std;

Camera::Camera() {
    rgbTexture.create(imageWidth, imageHeight);
    depthTexture.create(imageWidth, imageHeight);
    rgbPixels = new sf::Uint8[totalSize()];
    depthPixels = new sf::Uint8[totalSize()];
}

int Camera::totalSize() {
    return imageWidth * (imageHeight)*4;
}

pair<sf::Texture, sf::Texture> Camera::capture() {
    int i;
    for (auto h = 0; h < imageHeight; h++) {
        for (auto w = 0; w < imageWidth; w++) {
            i = w * imageHeight * 4 + h * 4;
            //auto i = h * imageWidth + w * 4;
            rgbPixels[i] = 100;
            rgbPixels[i + 1] = 50;
            rgbPixels[i + 2] = 100;
            rgbPixels[i + 3] = 255;
        }
    }
    //sf::Image rgb;
    //rgb.create(imageSize, imageSize/2, rgbPixels);
    //rgbTexture.update(rgb);
    //sf::Image depth;
    //depth.create(imageSize, imageSize/2, rgbPixels);
    //depthTexture.update(depth);
    rgbTexture.update(rgbPixels, imageWidth, imageHeight, 0, 0);
    depthTexture.update(depthPixels, imageWidth, imageHeight, 0, 0);
    return pair<sf::Texture, sf::Texture>(rgbTexture, depthTexture);
}

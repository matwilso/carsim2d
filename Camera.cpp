
#include "Camera.h"
#include <math.h>
#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <utility>

#include "Common.h"

using namespace std;

Camera::Camera(string name, b2Body* body, b2Vec2 offset, float angle) : name{name}, body{body}, offset{offset}, angle{angle} {
    rgbTexture.create(imageWidth, imageHeight);
    rgbPixels = new sf::Uint8[imageWidth * imageHeight * 4];
}

class RayCallback : public b2RayCastCallback {
   public:
    string name;
    sf::RenderWindow* window;
    b2Vec2 start;
    int idx;
    sf::Uint8 depthBuffer[64];
    sf::Color rgbBuffer[64];
    RayCallback(string name, sf::RenderWindow* window) : name{name}, window{window} {
        fill_n(depthBuffer, 64, 0);
        for (auto i = 0; i < 64; i++) {
            rgbBuffer[i] = sf::Color::Black;
        }
    }
    float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
        sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(5 * start.x, -5 * start.y), sf::Color::Red),
                sf::Vertex(sf::Vector2f(5 * point.x, -5 * point.y), sf::Color::Red),
            };
        window->draw(line, 2, sf::Lines);
        depthBuffer[idx] = sf::Uint8(255 - round(255 * fraction));
        auto ud = (FixtureUserData*)fixture->GetUserData().pointer;
        rgbBuffer[idx] = ud->color;
        sf::CircleShape shape(5.f);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(5 * point.x, -5 * point.y);
        window->draw(shape);
        return -1;
    }
};

sf::Texture Camera::capture(sf::RenderWindow* window) {
    RayCallback cb(name, window);
    auto t = body->GetTransform().q;

    auto ang = body->GetAngle() + angle;
    b2Mat22 rotMat(cos(ang), -sin(ang), sin(ang), cos(ang));
    auto offsetDelta = b2Mul(rotMat, offset);
    auto start = offsetDelta + body->GetWorldPoint(b2Vec2(0.0, 0.0));

    for (auto i = 0; i < samples; i++) {
        auto delta = -fov / 2 + (fov * i / samples);
        auto deltaAng = body->GetAngle() + angle + delta;
        auto end = start + range * b2Vec2(-sin(deltaAng), cos(deltaAng));
        cb.start = start;
        cb.idx = samples - i;
        body->GetWorld()->RayCast(&cb, start, end);
    }

    for (auto h = 0; h < imageHeight; h++) {
        for (auto w = 0; w < imageWidth; w++) {
            auto i = h * imageWidth * 4 + w * 4;
            auto col = cb.rgbBuffer[w / upsize];
            int depth = cb.depthBuffer[w / upsize];
            rgbPixels[i] = round((depth / 255.0f) * col.r);
            rgbPixels[i + 1] = round((depth / 255.0f) * col.g);
            rgbPixels[i + 2] = round((depth / 255.0f) * col.b);
            rgbPixels[i + 3] = 255;
        }
    }

    rgbTexture.update(rgbPixels, imageWidth, imageHeight, 0, 0);
    return rgbTexture;
}
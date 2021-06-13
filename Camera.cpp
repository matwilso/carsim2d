
#include "Camera.h"

#include <math.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <utility>

using namespace std;

Camera::Camera(string name, b2Body* body, b2Vec2 offset, float angle) {
    rgbTexture.create(imageWidth, imageHeight);
    depthTexture.create(imageWidth, imageHeight);
    rgbPixels = new sf::Uint8[totalSize()];
    depthPixels = new sf::Uint8[totalSize()];
    this->name = name;
    this->body = body;
    this->offset = offset;
    this->angle = angle;
}

int Camera::totalSize() {
    return imageWidth * (imageHeight)*4;
}

class RayCallback : public b2RayCastCallback {
   public:
    string name;
    sf::RenderWindow* window;
    b2Vec2 start;
    int idx;
    sf::Uint8 buffer[64];
    RayCallback(string name, sf::RenderWindow* window) {
        fill_n(buffer, 64, 0);
        this->name = name;
        this->window = window;
    }
    float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
        if (fraction > 0.1) {
            sf::Vertex line[] =
                {
                    sf::Vertex(sf::Vector2f(5 * start.x, -5 * start.y), sf::Color::Red),
                    sf::Vertex(sf::Vector2f(5 * point.x, -5 * point.y), sf::Color::Red),
                };
            window->draw(line, 2, sf::Lines);
            buffer[idx] = sf::Uint8(255-round(255*fraction));
            //sf::CircleShape shape(5.f);
            //shape.setFillColor(sf::Color::Red);
            //shape.setPosition(5 * point.x, -5 * point.y);
            //window->draw(shape);
            cout
                << name << " fixture " << fraction << endl;
            return 0;
        } else {
            //buffer[idx] = sf::Uint8(0);
            return 1;
        }
    }
};

pair<sf::Texture, sf::Texture> Camera::capture(sf::RenderWindow* window) {
    RayCallback cb(name, window);
    auto t = body->GetTransform().q;
    auto start = body->GetWorldPoint(b2Vec2(0.0, 0.0));
    for (auto i = 0; i < samples; i++) {
        auto delta = -fov / 2 + (fov * i / samples);
        auto deltaAng = body->GetAngle() + angle + delta;
        auto end = start + range * b2Vec2(-sin(deltaAng), cos(deltaAng));
        cb.start = start;
        cb.idx = samples-i;
        body->GetWorld()->RayCast(&cb, start, end);
    }

    for (auto h = 0; h < imageHeight; h++) {
        for (auto w = 0; w < imageWidth; w++) {
            auto i = h * imageWidth * 4 + w * 4;
            //auto i = w * imageHeight * 4 + h * 4;
            //auto i = h * imageWidth + w * 4;
            rgbPixels[i] = cb.buffer[w];
            rgbPixels[i + 1] = cb.buffer[w];
            rgbPixels[i + 2] = cb.buffer[w];
            rgbPixels[i + 3] = 255;
        }
    }

    //    auto e1 = start + camera->range * b2Vec2(-sin(camera->body->GetAngle() + camera->angle + camera->fov / 2), cos(camera->body->GetAngle() + camera->angle + camera->fov / 2));

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

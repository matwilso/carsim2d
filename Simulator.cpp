#include "Simulator.h"

#include <math.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>

#include "Constants.h"
#include "include/box2d/box2d.h"

using namespace std;

namespace render {
void fixture(b2Fixture* fix, sf::RenderWindow* window, sf::Color color = sf::Color(100, 250, 50)) {
    if (auto shape = dynamic_cast<b2PolygonShape*>(fix->GetShape())) {
        auto trans = fix->GetBody()->GetTransform();
        sf::ConvexShape convex;
        int n = shape->m_count;
        convex.setPointCount(n);
        for (auto i = 0; i < n; i++) {
            auto vert = fix->GetBody()->GetWorldPoint(shape->m_vertices[i]);
            sf::Vector2f arr(5.0 * vert.x, -5.0 * vert.y);
            //cout << "i " << i << " " << vert.x << " " << vert.y << endl;
            //cout << "i " << i << " " << arr.x << " " << arr.y << endl;
            convex.setPoint(i, arr);
        }
        convex.setFillColor(color);
        window->draw(convex);
    } else if (auto shape = dynamic_cast<b2EdgeShape*>(fix->GetShape())) {
        //cout << "LINE" << endl;
        /// TODO: go through the effort of adding thickness to this.
        auto trans = fix->GetBody()->GetTransform();
        sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(5.0 * shape->m_vertex1.x, -5.0 * shape->m_vertex1.y), color),
                sf::Vertex(sf::Vector2f(5.0 * shape->m_vertex2.x, -5.0 * shape->m_vertex2.y), color),
            };
        window->draw(line, 2, sf::Lines);
    }
}

void shape(b2Body* body, sf::RenderWindow* window, sf::Color color = sf::Color(100, 250, 50)) {
    auto fix = body->GetFixtureList();
    for (b2Fixture* fix = body->GetFixtureList(); fix; fix = fix->GetNext())
        fixture(fix, window, color);
}

}  // namespace render

Simulator::Simulator() {
    b2Vec2 gravity(0.0f, 0.0f);
    world = new b2World(gravity);
    //car = unique_ptr<Car>(new Car(world));
    car = new Car(world);
    b2EdgeShape edgeShape;

    b2BodyDef bodyDef;
    edge = world->CreateBody(&bodyDef);
    edgeShape.SetTwoSided(b2Vec2(100.0f, -100.0f), b2Vec2(100.0f, -200.0f));
    edge->CreateFixture(&edgeShape, 1.0);
    edgeShape.SetTwoSided(b2Vec2(50.0f, -100.0f), b2Vec2(100.0f, -100.0f));
    edge->CreateFixture(&edgeShape, 1.0);
    //edgeShape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(100.0f, -0100.0f));
    //edge->CreateFixture(&edgeShape, 1.0);
}

void Simulator::step(int controlState, sf::RenderWindow* window) {
    car->update(controlState);
    world->Step(dt, 10, 10);
    class RayCallback : public b2RayCastCallback {
       public:
        sf::RenderWindow* window;
        RayCallback(sf::RenderWindow* window_) {
            window = window_;
        }

        float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
            cout << "fixture" << fraction << endl;
            render::fixture(fixture, window);
            return 1;
        }
    };
    RayCallback cb(window);

    render::shape(edge, window);
    render::shape(car->body, window, sf::Color(155, 0, 0));
    for (auto wheel : car->wheels) {
        render::shape(wheel->body, window);
    }
    auto x = camera.capture();
    sf::Sprite sprite;
    sprite.setTexture(x.first);
    sprite.setPosition(100, 100);
    window->draw(sprite);
    world->RayCast(&cb, b2Vec2(0.0f, 0.0f), b2Vec2(100.0f, -100.0f));
}

Simulator::~Simulator() {
}
#include "Simulator.h"

#include <math.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>

#include "Constants.h"
#include "include/box2d/box2d.h"

using namespace std;

Simulator::Simulator() {
    b2Vec2 gravity(0.0f, 0.0f);
    world = new b2World(gravity);
    //car = unique_ptr<Car>(new Car(world));
    car = new Car(world);
}

void renderShape(b2Body* body, sf::RenderWindow& window, sf::Color color = sf::Color(100,250,50)) {
    auto fix = body->GetFixtureList()[0];
    if (auto shape = dynamic_cast<b2PolygonShape*>(fix.GetShape())) {
        auto trans = fix.GetBody()->GetTransform();
        sf::ConvexShape convex;
        int n = shape->m_count;
        convex.setPointCount(n);
        for (auto i = 0; i < n; i++) {
            auto vert = body->GetWorldPoint(shape->m_vertices[i]);
            sf::Vector2f arr(100 + 5.0 * vert.x, 100 - 5.0 * vert.y);
            cout << "i " << i << " " << arr.x << " " << arr.y << endl;
            convex.setPoint(i, arr);
        }
        convex.setFillColor(color);
        window.draw(convex);
    }
    //else if (auto shape = dynamic_cast<b2PolygonShape*>(fix.GetShape())) { }
}

void Simulator::step(int controlState, sf::RenderWindow& window) {
    car->update(controlState);
    world->Step(dt, 10, 10);
    renderShape(car->body, window, sf::Color(155, 0, 0));
    for (auto wheel : car->wheels) {
        renderShape(wheel->body, window);
    }
}

Simulator::~Simulator() {
}
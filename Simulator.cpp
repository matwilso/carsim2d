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
void camera(Camera* camera, sf::RenderWindow* window, sf::Color color = sf::Color(100, 250, 50)) {
    // create an array of 3 vertices that define a triangle primitive
    sf::VertexArray triangle(sf::Quads, 4);
    // define the position of the triangle's points

    auto t = camera->body->GetTransform().q;
    auto start = camera->body->GetWorldPoint(b2Vec2(0.0, 0.0));
    //auto start = camera->body->GetWorldPoint(camera->offset);
    auto e1 = start + 1.0 * camera->range * b2Vec2(-sin(camera->body->GetAngle() + camera->angle + camera->fov / 2), cos(camera->body->GetAngle() + camera->angle + camera->fov / 2));
    auto e2 = start + 1.0 * camera->range * b2Vec2(-sin(camera->body->GetAngle() + camera->angle), cos(camera->body->GetAngle() + camera->angle));
    auto e3 = start + 1.0 * camera->range * b2Vec2(-sin(camera->body->GetAngle() + camera->angle - camera->fov / 2), cos(camera->body->GetAngle() + camera->angle - camera->fov / 2));

    triangle[0].position = sf::Vector2f(5.0 * start.x, -5.0 * start.y);
    triangle[1].position = sf::Vector2f(5.0 * e1.x, -5.0 * e1.y);
    triangle[2].position = sf::Vector2f(5.0 * e2.x, -5.0 * e2.y);
    triangle[3].position = sf::Vector2f(5.0 * e3.x, -5.0 * e3.y);

    // define the color of the triangle's points
    triangle[0].color = sf::Color(100, 250, 50, 30);
    triangle[1].color = sf::Color(100, 250, 50, 30);
    triangle[2].color = sf::Color(100, 250, 50, 30);
    triangle[3].color = sf::Color(100, 250, 50, 30);
    window->draw(triangle);
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
    edgeShape.SetTwoSided(b2Vec2(100.0f, -125.0f), b2Vec2(100.0f, -200.0f));
    edge->CreateFixture(&edgeShape, 1.0);
    edgeShape.SetTwoSided(b2Vec2(50.0f, -125.0f), b2Vec2(100.0f, -125.0f));
    edge->CreateFixture(&edgeShape, 1.0);
    //edgeShape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(100.0f, -0100.0f));
    //edge->CreateFixture(&edgeShape, 1.0);
    frontCam = new Camera("front"s, car->body, b2Vec2(0, 10.0f), 0.0);
    //rightCam = new Camera("right"s, car->body, b2Vec2(0, 0), 1.5707);
    //camera = Camera(car->body, b2Vec2(0, 0), 0.0);
}

void Simulator::step(int controlState, sf::RenderWindow* window) {
    car->update(controlState);
    world->Step(dt, 10, 10);

    render::shape(edge, window);
    render::shape(car->body, window, sf::Color(155, 0, 0));
    for (auto wheel : car->wheels) {
        render::shape(wheel->body, window);
    }
    render::camera(frontCam, window);
    //rightCam->capture();
    auto x = frontCam->capture(window);
    sf::Sprite sprite;
    sprite.setTexture(x.first);
    sprite.setPosition(800, 10);
    window->draw(sprite);
}

Simulator::~Simulator() {
}

#include "Simulator.h"

#include <math.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>

#include "Common.h"
#include "include/box2d/box2d.h"

using namespace std;

namespace render {
void fixture(b2Fixture* fix, sf::RenderWindow* window, sf::Color color = sf::Color(100, 250, 50)) {
    auto ud = (FixtureUserData*)fix->GetUserData().pointer;
    //ud.pointer
    //auto x = (FixtureUserData*)ud.pointer;
    //FixtureUserData* ud = dynamic_cast<FixtureUserData*>(&(fix->GetUserData()));

    if (auto shape = dynamic_cast<b2PolygonShape*>(fix->GetShape())) {
        auto trans = fix->GetBody()->GetTransform();
        sf::ConvexShape convex;
        int n = shape->m_count;
        convex.setPointCount(n);
        for (auto i = 0; i < n; i++) {
            auto vert = fix->GetBody()->GetWorldPoint(shape->m_vertices[i]);
            sf::Vector2f arr(5.0 * vert.x, -5.0 * vert.y);
            convex.setPoint(i, arr);
        }
        convex.setFillColor(ud->color);
        window->draw(convex);
    } else if (auto shape = dynamic_cast<b2EdgeShape*>(fix->GetShape())) {
        //cout << "LINE" << endl;
        /// TODO: go through the effort of adding thickness to this.
        auto trans = fix->GetBody()->GetTransform();
        sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(5.0 * shape->m_vertex1.x, -5.0 * shape->m_vertex1.y), ud->color),
                sf::Vertex(sf::Vector2f(5.0 * shape->m_vertex2.x, -5.0 * shape->m_vertex2.y), ud->color),
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
    auto ang = camera->body->GetAngle() + camera->angle;
    b2Mat22 rotMat(cos(ang), -sin(ang), sin(ang), cos(ang));
    auto delta = b2Mul(rotMat, camera->offset);
    //auto start = camera->body->GetWorldPoint(camera->offset);
    auto start = delta + camera->body->GetWorldPoint(b2Vec2(0.0, 0.0));

    auto e1 = start + 1.0 * camera->range * b2Vec2(-sin(camera->body->GetAngle() + camera->angle + camera->fov / 2), cos(camera->body->GetAngle() + camera->angle + camera->fov / 2));
    auto e2 = start + 1.0 * camera->range * b2Vec2(-sin(camera->body->GetAngle() + camera->angle), cos(camera->body->GetAngle() + camera->angle));
    auto e3 = start + 1.0 * camera->range * b2Vec2(-sin(camera->body->GetAngle() + camera->angle - camera->fov / 2), cos(camera->body->GetAngle() + camera->angle - camera->fov / 2));

    triangle[0].position = sf::Vector2f(5.0 * start.x, -5.0 * start.y);
    triangle[1].position = sf::Vector2f(5.0 * e1.x, -5.0 * e1.y);
    triangle[2].position = sf::Vector2f(5.0 * e2.x, -5.0 * e2.y);
    triangle[3].position = sf::Vector2f(5.0 * e3.x, -5.0 * e3.y);

    // define the color of the triangle's points
    triangle[0].color = sf::Color(255, 50, 50, 30);
    triangle[1].color = sf::Color(255, 50, 50, 30);
    triangle[2].color = sf::Color(255, 50, 50, 30);
    triangle[3].color = sf::Color(255, 50, 50, 30);
    window->draw(triangle);
}
}  // namespace render

//class MyDestructionListener : public b2DestructionListener {
//    void SayGoodbye(b2Fixture* fixture) {
//        if (FixtureUserData* fud = (FixtureUserData*)fixture->GetUserData())
//            delete fud;
//    }
//    //(unused but must implement all pure virtual functions)
//    void SayGoodbye(b2Joint* joint) {}
//};

Simulator::Simulator() {
    b2Vec2 gravity(0.0f, 0.0f);
    world = new b2World(gravity);
    //car = unique_ptr<Car>(new Car(world));
    car = new Car(world);

    b2PolygonShape edgeShape;
    b2FixtureDef fixDef;
    fixDef.density = 1.0;
    fixDef.shape = &edgeShape;
    b2BodyDef bodyDef;
    edge = world->CreateBody(&bodyDef);

    // TODO: dealloc the fixturedata

    auto plotVLine = [&](float x, float y1, float y2, sf::Color color) {
        b2Vec2 arr[4];
        arr[0] = b2Vec2(x, y1);
        arr[1] = b2Vec2(x, y2);
        arr[2] = b2Vec2(x + 1, y2);
        arr[3] = b2Vec2(x + 1, y1);
        edgeShape.Set(arr, 4);
        auto ud = new FixtureUserData;
        ud->color = color;
        fixDef.userData.pointer = reinterpret_cast<uintptr_t>(ud);
        //b2Fixture* fixture = body->CreateFixture(&fixDef);  //shape, density
        //fixDef.userData = FixtureUserData(sf::Color::Blue);
        edge->CreateFixture(&fixDef);
    };
    auto plotHLine = [&](float x1, float x2, float y, sf::Color color) {
        b2Vec2 arr[4];
        arr[0] = b2Vec2(x1, y);
        arr[1] = b2Vec2(x2, y);
        arr[2] = b2Vec2(x2, y + 1);
        arr[3] = b2Vec2(x1, y + 1);
        edgeShape.Set(arr, 4);
        auto ud = new FixtureUserData;
        ud->color = color;
        fixDef.userData.pointer = reinterpret_cast<uintptr_t>(ud);
        //b2Fixture* fixture = body->CreateFixture(&fixDef);  //shape, density
        //fixDef.userData = FixtureUserData(sf::Color::Blue);
        edge->CreateFixture(&fixDef);

        //fixDef.userData = FixtureUserData(sf::Color::Red);
        //edge->CreateFixture(&fixDef);
    };
    plotHLine(30.0f, 100.0f, -50.0f, sf::Color::Yellow);
    plotHLine(30.0f, 200.0f, -100.0f, sf::Color::Green);
    plotHLine(150.0f, 200.0f, -50.0f, sf::Color::Blue);
    plotVLine(150.0f, -50.0f, -10.0f, sf::Color::Yellow);
    plotHLine(0.0, 150.0, -10.0f, sf::Color::Cyan);
    plotVLine(200.0f, -100.0f, -50.0f, sf::Color::Magenta);
    plotVLine(1.0f, -100.0f, -10.0f, sf::Color::Magenta);
    frontCam = new Camera("front"s, car->body, b2Vec2(0, 10.0f), 0.0);
}

void Simulator::step(int controlState, sf::RenderWindow* window) {
    car->update(controlState);
    world->Step(dt, 10, 10);

    render::shape(edge, window);
    render::shape(car->body, window, sf::Color(200, 50, 50));
    for (auto wheel : car->wheels) {
        render::shape(wheel->body, window, sf::Color::Black);
    }
    render::camera(frontCam, window);
    auto x = frontCam->capture(window);
    sf::Sprite rgbSprite;
    rgbSprite.setTexture(x.first);
    rgbSprite.setPosition(800, 10);
    window->draw(rgbSprite);
    //sf::Sprite depthSprite;
    //depthSprite.setTexture(x.second);
    //depthSprite.setPosition(800, 138);
    //window->draw(depthSprite);

}

Simulator::~Simulator() {
    //cleanup
    for (auto bod = world->GetBodyList(); bod; bod = bod->GetNext()) {
        for (auto fix = bod->GetFixtureList(); fix; fix = fix->GetNext()) {
            delete (FixtureUserData*)fix->GetUserData().pointer;
            bod->DestroyFixture(fix);
        }
    }
}
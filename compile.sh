#!/usr/bin/env bash
g++ main.cpp Car.cpp -o sfml-app -Iinclude/ -Llib/ -lsfml-graphics -lsfml-window -lsfml-system -lbox2d -std=c++17

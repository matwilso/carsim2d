#!/usr/bin/env bash
g++ -w main.cpp Car.cpp Simulator.cpp -o sfml-app -Iinclude/ -Llib/ -lsfml-graphics -lsfml-window -lsfml-system -lbox2d -std=c++17

#!/usr/bin/env bash
g++ -c main.cpp Car.cpp
g++ main.o Car.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
./sfml-app

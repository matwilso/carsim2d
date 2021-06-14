# carsim2d

Simple top-down 2D car simulator made in C++ using box2D for physics and SFML for rendering.

This project is to dust off my C++ knowledge and get practice developing a Software 1.0 simulator to better understand it
(the [Chestertons's Fence](https://wiki.lesswrong.com/wiki/Chesterton%27s_Fence)).

It is pretty barebones, so don't get that excited.

(TODO: video)

## Install

This repo only supports ubuntu. I precompiled box2d as a static library and included it here.

To install SFML:
`sudo apt-get install libsfml-dev`

The scripts `compile.sh` and `run.sh` show how to compile things.

## References

For the initial car physics: http://www.iforce2d.net/b2dtut/top-down-car

The rendering idea is loosely based off: https://andyljones.com/megastep/


## TODO

- road maker / scene generator tools
- other moving agents
- varying lighting
# carsim2d

Simple top-down 2D car simulator made in C++ using box2D for physics and SFML for rendering.

The purpose of this project is to dust off my C++ knowledge and get practice developing a Software 1.0 simulator to better understand it
(like a [Chestertons's Fence](https://en.wikipedia.org/wiki/Wikipedia:Chesterton's_fence)).

It is pretty barebones, so don't get that excited.




https://user-images.githubusercontent.com/19582683/121852796-38d7b000-ccad-11eb-8dab-3e283a47bae1.mp4





## References

For the initial car physics: http://www.iforce2d.net/b2dtut/top-down-car

For vision sensor, I use box2d's ray-tracing to create a RGB image which drops off with depth. The idea is loosely based off: https://andyljones.com/megastep/


## TODO

- road maker / scene generator tools
- other moving agents
- varying lighting?

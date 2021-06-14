# carsim2d

Simple top-down 2D car simulator made in C++ using box2D for physics and SFML for rendering.

This was a weekend project to dust off my C++ knowledge and practice developing a Software 1.0 simulator to [better understand](https://en.wikipedia.org/wiki/Wikipedia:Chesterton's_fence) the principles.

It is pretty barebones, so don't get that excited.




https://user-images.githubusercontent.com/19582683/121852796-38d7b000-ccad-11eb-8dab-3e283a47bae1.mp4





## References

For the initial car physics: http://www.iforce2d.net/b2dtut/top-down-car

For vision sensor, I use box2d's ray-tracing to create a 1D RGB image which drops off with depth. The idea is loosely based off: https://andyljones.com/megastep/


## TODO

- road maker / scene generator tools
- other moving agents
- varying lighting?

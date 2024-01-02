# Overview

This is my implementation of the [Pikuma 2D Game Engine with C++ and Lua](https://courses.pikuma.com/courses/take/2dgameengine) course.
* The code is pretty hacky for now.
* I used [cmake](https://cmake.org) instead of old school make used in the course.
* I used a coding style I prefer.
* I diverged from the example implementation is small ways like the use of pointers and some numeric types. 

I initially made quite a few changes but ended up just following along until I complete the course because I wanted to learn the patterns etc and the rest can be cleaned up later.

# Building

I've only built this on macOS for now.

```
brew install sdl2
brew install sdl2_image
brew install sdl2_ttf
brew install sdl2_mixer
brew install lua
```
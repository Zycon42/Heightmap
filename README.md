Heightmap
==============

Heightmap rendering with some LOD techniques

dependencies
------------

  * gcc >= 4.7 || msvc >= 12
  * OpenGL 4.2.
  * GLEW
  * GLM
  * SDL 2
  * Boost 1.55.

build
-----

This project is using cmake.

GCC:

  * `mkdir build && cd build`
  * `cmake ..`
  * `make`
  
MSVC:

  * `mkdir build && cd build`
  * `cmake -G Visual Studio 12`
  * Open MSVC and build solution

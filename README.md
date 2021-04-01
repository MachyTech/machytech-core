# CX Laser Projector

core module used on the laserprojector of MachyTech. Superfast C++ API for rendering scenes in front of an autonomous robot.

## Getting started
```
mkdir build && cd build
```
```
cmake ../ && make -j4
```
```
./app
``` 

### git submodules
we are using glfw and glmath that you need to install as git submodules.
```
git pull --recurse-submodules
```

## new features
* filemanagement

Reading a csv file and store in the correct format for openGL.

* path scene

This feature includes code for the rendering of the csv file in openGL.

* libcurl file reader from http server

Reading remote csv files FAST!! using libcurl. 

* Linux environment variables

Change often used variables without having to recompile. Simpy export the variable to your current linux proccess. Currently supported for:
```
GLSL_APP_VERT #vertex shader directory
GLSL_APP_FRAG #fragment shader directory
IP_ADDR #ip
PORT_ADDR #port
```

* systemD service

.conf file for systemd service

## Future work

* Creating extra scenes
* Read odometry data from sensors
* Netwerk device driver(?)
* CI/CD (jenkins?) to Beaglebone
* logging

## Resources
* Hands-on Network Programming with C: Learn Socket Programming in C

Learn socket programming in C and write secure and optimized network code.

* openGL SuperBible

Usefull to get some context of graphics and the GPU. Very advanced.

* The C++ programming language: Bjarne Stroustrump

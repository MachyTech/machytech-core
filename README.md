# NO MAIN program only samples (needs more testing & debugging)
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
### cmake
I prefer using latest cmake
```
curl -OL https://github.com/Kitware/CMake/releases/download/v3.20.1/cmake-3.20.1-SHA-256.txt
curl -OL https://github.com/Kitware/CMake/releases/download/v3.20.1/cmake-3.20.1.tar.gz
sha256sum -c --ignore-missing cmake-3.20.1-SHA-256.txt
```
If this returns ok unzip folder and make install
```
tar -zxvf cmake-3.20.1.tar.gz
cmake .
make
make install

### BOOST
you need a very specific version of boost. BOOST 1.58.0
```
sudo apt-get -y install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev
wget http://downloads.sourceforge.net/project/boost/boost/1.54.0/boost_1_54_0.tar.gz
tar -zxvf boost_1_58_0.tar.gz
./bootstrap.sh
./b2 --with=all -j $cpuCores install
```

```
### git submodules
we are using glfw and glmath that you need to install as git submodules.
```
git submodule update --init --recursive
```

### build image
If you dont want to actually build the image you can also pull the image from our machytech docker hub repository.
```
docker run -it --mount type=bind,source=<path-to-target>,target=/home docker.io/machytech/armv7-build@sha256:f4f929ea0a0c451e0407b63fc2949cda6f3d335fc73c5068df483788b97f85f8 /bin/bash
```
build the image using the dockerfile. Assuming you have docker and buildx installed. Documentation is very good so check for latest installation instruction on their website. I have installed buildx from their github page, I would advise doing this as well.
To create a build instance for buildx do the following.
```
docker buildx create --name machytech-core-build
docker buildx create use machytech-core-build
```
Now run the docker file as machytech-core-build user
```
docker buildx build --platform linux/arm,linux/arm64 -t machytech/armv7-build . --push
```
This builds an armv7 and arm64 image and pushes it to machytech's docker hub.  
You can check the names of the created images here
```
docker buildx imagetools inspect machytech/armv7-build
```
Now copy the name from the armv7 image and run the container with a bash instance. Also mount the source code.
```
docker run -it --mount type=bind,source=<path-to-source>,target=/home <name> /bin/bash
```

## new features
* MachyAPI (14/06/2021)
minimum working version of the machyAPI.
    * tcp trajectory server
* Vertical window (29/05/2021)
* White background (29/05/2021)
* ARMv7 build image (05/05/2021)

docker build image ARMv7 with debian and depencies pre-installed

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
SCENE #used scene
TCP_IP #tcp ip address
TCP_PORT # tcp port address
HTTP_IP # http ip address
HTTP_PORT # http port address
HTTP_ROUTE # http route
CURL_WEBURL # web url to data file (for robot path)
SAMPLE_SIZE # number of sample used from robot path
LINEWIDTH # width of line
WINDOW_WIDTH # width of openGL rendering context
WINDOW_HEIGHT # height of openGL rendering context
```
* systemD service

.conf file for systemd service

## Future work

* machyAPI (RESTful)
* Default scene on boot beaglebone
* DEB packaging
* Creating extra scenes
* Read odometry data from sensors
* CI/CD (jenkins?) to Jetson
* logging

## Resources
* Hands-on Network Programming with C: Learn Socket Programming in C

Learn socket programming in C and write secure and optimized network code.

* openGL SuperBible

Usefull to get some context of graphics and the GPU. Very advanced.

* The C++ programming language: Bjarne Stroustrump

libcurl development

* https://curl.se/dev/

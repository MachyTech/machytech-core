cmake_minimum_required(VERSION 3.10)

# name of the project
project(app)

# enable language
enable_language(CXX)

# cross-compiling toolchain
#set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_SYSTEM_PROCESSOR arm)

#set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabi-gcc)
#set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabi-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# GLFW
# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOLL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/glfw)

# include shaders
file(COPY shader DESTINATION .)

# test csv file
file (COPY src/trajectory_1_fpg_out.txt DESTINATION .)

# header files
include_directories(include)
include_directories(vendor/glm)

# sources
file(GLOB SOURCES "src/*.cpp")

# find curl
find_package( CURL REQUIRED )

# link libraries
ADD_LIBRARY(LibsModule ${SOURCES})
target_link_libraries(LibsModule -lglfw3)
target_link_libraries(LibsModule -lX11)
target_link_libraries(LibsModule -lpthread)
target_link_libraries(LibsModule -lXrandr)
target_link_libraries(LibsModule -ljson-c)
target_link_libraries(LibsModule CURL::libcurl)
target_link_libraries(LibsModule glfw)

# create executable
add_executable(app ${SOURCES})
target_link_libraries(app LibsModule)

install(TARGETS app
    COMPONENT projectorapp
    RUNTIME DESTINATION "/usr/bin"
    LIBRARY DESTINATION "/usr/bin/"
    DESTINATION "/usr/bin/"
)

# create .deb package
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "kk")
INCLUDE(CPack)

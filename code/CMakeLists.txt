cmake_minimum_required(VERSION 3.10)

# name of the project
project(app)

# enable language
enable_language(CXX)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

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
target_link_libraries(LibsModule -lX11)
target_link_libraries(LibsModule -lpthread)
target_link_libraries(LibsModule -lXrandr)
target_link_libraries(LibsModule -ljson-c)
target_link_libraries(LibsModule -lcurl)
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

cmake_minimum_required(VERSION 3.5)

# name of the project
project(boosthttpserver)

# enable language
enable_language(CXX)
set (CMAKE_CXX_STANDARD 11)
# header files
include_directories(include)

# sources
file(GLOB SOURCES "src/*.cpp")

# link libraries
# boost
find_package( Boost 1.58 REQUIRED system )
include_directories(${Boost_INCLUDE_DIRS})

# examples
if(BUILD_EXAMPLES)
  add_executable(client "samples/client.cpp" ${SOURCES})
  target_link_libraries(client ${Boost_LIBRARIES})
  add_executable(server "samples/server.cpp" ${SOURCES})
  target_link_libraries(server ${Boost_LIBRARIES})
endif()
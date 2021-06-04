cmake_minimum_required(VERSION 3.20.1)

# name of the project
project(boosthttpserver)

# enable language
enable_language(CXX)
set (CMAKE_CXX_STANDARD 11)
# pthread
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_NO_CXX11_SCOPED_ENUMS")
# header files
include_directories(include)

# sources
file(GLOB SOURCES "src/*.cpp")

# link libraries
# boost
find_package( Boost 1.58 REQUIRED system )
include_directories(${Boost_INCLUDE_DIRS})

# use boost enhanced functions(?)
if(BOOST_ENHANCED)
  add_compile_definitions(BOOST_ENHANCED)
endif()

# examples
if(BUILD_EXAMPLES)
  add_executable(sync_client "samples/sync_client.cpp" ${SOURCES})
  target_link_libraries(sync_client ${Boost_LIBRARIES})
  add_executable(sync_server "samples/sync_server.cpp" ${SOURCES})
  target_link_libraries(sync_server ${Boost_LIBRARIES})
  add_executable(async_client "samples/async_client.cpp" ${SOURCES})
  target_link_libraries(async_client ${Boost_LIBRARIES})
  add_executable(async_server "samples/async_server.cpp" ${SOURCES})
  target_link_libraries(async_server ${Boost_LIBRARIES})
  add_executable(async_httpserver "samples/async_httpserver.cpp" ${SOURCES})
  target_link_libraries(async_httpserver ${Boost_LIBRARIES})
endif()
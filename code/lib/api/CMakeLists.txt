cmake_minimum_required(VERSION 3.20.1)

# name of the project
project(machyapi VERSION 0.0.1 DESCRIPTION "machyapi functions used by machytechs projecting system")

add_library(machyapi SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/api.cpp)

set_target_properties(machyapi PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(machyapi PROPERTIES SOVERSION 0.0.1)

target_include_directories(machyapi PRIVATE ${CMAKE_SOURCE_DIR}/lib/api/include)

# headers
include_directories(${CMAKE_SOURCE_DIR}/lib/machycore/include)

# enable language (we heavily use c++ 11)
# enable_language(CXX)
# set (CMAKE_CXX_STANDARD 11)

# pthread
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# find_package( Boost 1.58 EXACT REQUIRED system )
# include_directories(${Boost_INCLUDE_DIRS})
#if (NOT TARGET machycore)
    # add_subdirectory(${CMAKE_SOURCE_DIR}/lib/machycore lib/machycore)
# endif()

# include_directories(${CMAKE_SOURCE_DIR}/lib/machycore/include)
# include_directories(${CMAKE_SOURCE_DIR}/lib/machyapi/include)

# target_link_libraries(machyapi machycore)

# # use boost enhanced functions(?)
# if(BOOST_ENHANCED)
#   add_compile_definitions(BOOST_ENHANCED)
# endif()

# # examples
# if(BUILD_EXAMPLES)
#   add_executable(sync_client "samples/sync_client.cpp" ${SOURCES} "")
#   target_link_libraries(sync_client ${Boost_LIBRARIES})
#   add_executable(sync_server "samples/sync_server.cpp" ${SOURCES})
#   target_link_libraries(sync_server ${Boost_LIBRARIES})
#   add_executable(async_client "samples/async_client.cpp" ${SOURCES})
#   target_link_libraries(async_client ${Boost_LIBRARIES})
#   add_executable(async_server "samples/async_server.cpp" ${SOURCES})
#   target_link_libraries(async_server ${Boost_LIBRARIES})
#   add_executable(async_httpserver "samples/async_httpserver.cpp" ${SOURCES})
#   target_link_libraries(async_httpserver ${Boost_LIBRARIES})
# endif()

# ---------------------------------------------------------
#   Install
# ---------------------------------------------------------
install(TARGETS machyapi
        EXPORT machyapiTargets
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

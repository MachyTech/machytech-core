cmake_minimum_required(VERSION 3.10)

# name of the project
project(trajectorysim)

# enable language
enable_language(CXX)

#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# adding subdirectories
if (NOT TARGET glfw)
    add_subdirectory(vendor/glfw)
endif()
if (NOT TARGET machyapi)    
    add_subdirectory(lib/api)
endif()
if (NOT TARGET machycore)
    add_subdirectory(lib/machycore)
endif()
#if (NOT TARGET machygl)
#    add_subdirectory(lib/machygl)
#endif()
#if (NOT TARGET machyscenes)
#    add_subdirectory(lib/machyscenes)
#endif()
# include shaders
file(COPY shader DESTINATION .)

# test csv file
# file (COPY src/trajectory_1_fpg_out.txt DESTINATION .)

# header files
include_directories(include)
include_directories(vendor/glm)
include_directories(lib/machyscenes/include)
include_directories(lib/machygl/include)

# sources
file(GLOB SOURCES "src/*.cpp")

# find curl
# find_package( CURL REQUIRED )

# link libraries
ADD_LIBRARY(LibsModule ${SOURCES})
target_link_libraries(LibsModule -lX11)
target_link_libraries(LibsModule -lpthread)
target_link_libraries(LibsModule -lXrandr)
# target_link_libraries(LibsModule -ljson-c)
# target_link_libraries(LibsModule -lcurl)
target_link_libraries(LibsModule glfw)
#target_link_libraries(LibsModule glad)
# machytech
target_link_libraries(LibsModule machyapi)
target_link_libraries(LibsModule machycore)
#target_link_libraries(LibsModule machyscenes)
#target_link_libraries(LibsModule machygl)

# create executable
#add_executable(app ${SOURCES})
#target_link_libraries(app LibsModule)

# samples
add_executable(trajectorysim ${SOURCES} "samples/trajsim.cpp")
target_link_libraries(trajectorysim LibsModule)
add_executable(plotdata ${SOURCES} "samples/plotdata.cpp" )
target_link_libraries(plotdata LibsModule)

install(TARGETS trajectorysim
    COMPONENT projectorapp
    RUNTIME DESTINATION "/usr/bin"
    LIBRARY DESTINATION "/usr/bin/"
    DESTINATION "/usr/bin/"
)

# # create .deb package
# SET(CPACK_GENERATOR "DEB")
# SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "kk")
# INCLUDE(CPack)

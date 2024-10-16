cmake_minimum_required(VERSION 3.30)
project(slicer)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG "master"
        GIT_PROGRESS TRUE
)

#FetchContent_Declare(
#        serial
#        GIT_REPOSITORY "https://github.com/wjwwood/serial.git"
#        GIT_TAG "main"
#        GIT_PROGRESS TRUE
#)

FetchContent_MakeAvailable(raylib)
#FetchContent_MakeAvailable(serial)

add_executable(slicer main.cpp)
target_link_libraries(slicer PRIVATE raylib)
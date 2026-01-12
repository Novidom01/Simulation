cmake_minimum_required(VERSION 4.0)
project(untitled19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS_RELEASE "-static")
add_executable(untitled19 main.cpp
        physics_models.h
        physics_models.cpp
        file_io.h
        file_io.cpp
        menu.h
        menu.cpp)
target_link_libraries(untitled19
        -static-libgcc
        -static-libstdc++
)
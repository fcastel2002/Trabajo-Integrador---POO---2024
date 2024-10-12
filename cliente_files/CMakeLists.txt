cmake_minimum_required(VERSION 3.29)
project(cliente_files)

set(CMAKE_CXX_STANDARD 23)

add_executable(cliente_files main.cpp
        Cliente.cpp
        Cliente.h
        Orden.cpp
        Orden.h
        FileManager.cpp
        FileManager.h)

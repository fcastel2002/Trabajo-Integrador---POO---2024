cmake_minimum_required(VERSION 3.29)
project(cliente_files)

set(CMAKE_CXX_STANDARD 23)

add_executable(cliente_files main.cpp
        Command.h
        ActivarEfectorCommand.cpp
        ActivarEfectorCommand.h
        MoveEfectorCommand.cpp
        MoveEfectorCommand.h
        Cliente.cpp
        Cliente.h)

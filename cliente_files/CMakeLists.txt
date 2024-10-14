cmake_minimum_required(VERSION 3.29)
project(cliente_files)

set(CMAKE_CXX_STANDARD 23)
set(XMLRPCPP_LIB_PATH "XMLRPC++")

add_executable(cliente_files main.cpp
        Cliente.cpp
        Cliente.h
        FileManager.cpp
        FileManager.h)
target_link_libraries(cliente_files ./XMLRPC++/XMLRPC++.lib)

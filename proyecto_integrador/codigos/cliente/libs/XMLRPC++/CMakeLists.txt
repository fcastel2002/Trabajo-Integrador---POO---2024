# Establece la versión mínima de CMake
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(XMLRPC++)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir directorios de encabezados
include_directories(${CMAKE_SOURCE_DIR})

# Definir las opciones de compilación (directivas)
if(MINGW)
    add_definitions(-D_DEBUG -D_CONSOLE -D_CRT_SECURE_NO_WARNINGS)
endif()

# Obtener todos los archivos fuente
file(GLOB XMLRPC_SOURCES
    "${CMAKE_SOURCE_DIR}/*.cpp"
)

# Crear la biblioteca estática
add_library(xmlrpc++ STATIC ${XMLRPC_SOURCES})

# Vincular la biblioteca de sockets de Windows (ws2_32)
target_link_libraries(xmlrpc++ ws2_32)
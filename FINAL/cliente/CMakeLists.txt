cmake_minimum_required(VERSION 3.28)
project(cliente)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directorios de inclusión
include_directories(inc libs/PDCurses-3.8 libs/XMLRPC++)

# Archivos fuente
add_executable(cliente
    src/Archivo.cpp
    src/Cliente.cpp
    src/ErrorHandler.cpp
    src/main.cpp
    src/MainMenu.cpp
    src/PantallaCurses.cpp
)

# Vinculación de bibliotecas
target_link_libraries(cliente
    kernel32 user32 gdi32 winspool comdlg32 advapi32
    shell32 ole32 oleaut32 uuid odbc32 odbccp32
    ${CMAKE_SOURCE_DIR}/libs/PDCurses-3.8/wincon/pdcurses.a
    ${CMAKE_SOURCE_DIR}/libs/XMLRPC++/bins/libxmlrpc++.a
    ws2_32  
)

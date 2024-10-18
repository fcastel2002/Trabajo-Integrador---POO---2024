#ifndef MAINMENU_H
#define MAINMENU_H

#include "Cliente.h"
#include <curses.h> // Librería PDCurses
#include <string>
#pragma comment(lib, "pdcurses.a")
#pragma comment(lib, "MSVCRTD.lib")
#pragma comment(lib, "libcmtd.lib")  

class MainMenu {
public:
    MainMenu(Cliente& cliente);
    void mostrarMenu();

private:
    Cliente& cliente;
    void manejarSeleccion(int seleccion);
    void inicializarPantalla();
    void terminarPantalla();
};

#endif

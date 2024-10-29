#ifndef MAINMENU_H
#define MAINMENU_H

#include "Cliente.h"

#include "OrdenBuilder.h"

#include "IPantalla.h"
#include <curses.h> // Librería PDCurses
#include <string>
#include "Orden.h"
#include <vector>

#pragma comment(lib, "pdcurses.a")
#pragma comment(lib, "MSVCRTD.lib")
#pragma comment(lib, "libcmtd.lib")  

class MainMenu {
public:
    MainMenu(Cliente& cliente, IPantalla* pantalla);
    void mostrarMenu(ErrorHandler& errorHandler);
    void setComandos(const std::vector<std::string>& comandos);
    bool procesarSeleccion(int seleccion, ErrorHandler& errorHandler);


private:
    Cliente& cliente;
    IPantalla* m_pantalla;
    const std::string manejarSeleccion(int seleccion);

    std::vector<std::string> m_comandos;
};

#endif

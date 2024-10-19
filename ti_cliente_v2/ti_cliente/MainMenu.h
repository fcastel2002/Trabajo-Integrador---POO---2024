#ifndef MAINMENU_H
#define MAINMENU_H

#include "Cliente.h"
#include <curses.h> // Librería PDCurses
#include <string>
#include "Orden.h"
#include <vector>
#pragma comment(lib, "pdcurses.a")
#pragma comment(lib, "MSVCRTD.lib")
#pragma comment(lib, "libcmtd.lib")  

class MainMenu {
public:
    MainMenu(Cliente& cliente);
    void mostrarMenu();
    
	Orden crearOrden(const std::string& tipo);
    void setComandos(const std::vector<const std::string>& comandos);
private:
    Cliente& cliente;
    const std::string manejarSeleccion(int seleccion);
    void inicializarPantalla();
    void terminarPantalla();
    std::vector<const std::string> m_comandos;
};

#endif

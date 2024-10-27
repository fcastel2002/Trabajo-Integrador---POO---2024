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
    WINDOW* debugWin;
	Orden crearOrden(const std::string& tipo);
    void setComandos(const std::vector<std::string>& comandos);

    void mostrarMensajeDepuracion(const std::string& mensaje) {
        wprintw(debugWin, "%s\n", mensaje.c_str()); // Imprime el mensaje en la ventana de depuración
        wrefresh(debugWin); // Actualiza la ventana de depuración
    
    }

private:
    Cliente& cliente;
    const std::string manejarSeleccion(int seleccion);
    void inicializarPantalla();
    void terminarPantalla();
    std::vector<std::string> m_comandos;
};

#endif

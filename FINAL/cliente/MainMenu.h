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
    void mostrarMenu();
    void setComandos();
    bool procesarSeleccion(int seleccion);
    std::string procesarSeleccionLocal(int selecion);

private:
    Cliente& cliente;
    IPantalla* m_pantalla;
    const std::string manejarSeleccion(int seleccion, const std::string& para);

    const std::vector<std::string> m_opcionesCliente = {
        "Login",
        "Mostrar comandos",
        "Salir"
    };
    std::vector<std::string> m_comandos;

	std::vector<std::string> m_opciones; // Opciones del menú
    bool m_flagMenu = true;
};

#endif

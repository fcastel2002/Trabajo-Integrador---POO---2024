#ifndef MAINMENU_H
#define MAINMENU_H

#include "Cliente.h"
#include "OrdenBuilder.h"
#include "IPantalla.h"
#include "ErrorHandler.h" 
#include "Archivo.h"        
#include <curses.h>       
#include <string>
#include <vector>

//#pragma comment(lib, "pdcurses.a")
//#pragma comment(lib, "MSVCRTD.lib")
//#pragma comment(lib, "libcmtd.lib")

class MainMenu {
public:
    // Constructor que inicializa el men� principal con el cliente y la pantalla
    MainMenu(Cliente& cliente, IPantalla* pantalla);

    // Muestra el men� principal
    void mostrarMenu();

    // Configura los comandos del cliente
    void setComandos();

    // Procesa la selecci�n de comandos
    bool procesarSeleccion(int seleccion);

    //Sobre carga para procesar en local
    std::string procesarSeleccion(int seleccion, const std::string& quien);

private:
    Cliente& cliente;                     // Referencia al cliente para ejecutar comandos
    IPantalla* m_pantalla;                // Referencia a la pantalla para mostrar el men� y mensajes

    // Maneja la selecci�n en el men� principal o en el men� de comandos
    const std::string manejarSeleccion(int seleccion, const std::string& para);

    // Opciones del men� principal del cliente
     std::vector<std::string> m_opcionesCliente = {
        "Login",
        "Mostrar comandos",
        "Salir"
    };

    std::vector<std::string> m_comandos;  // Lista de comandos del cliente
    std::vector<std::string> m_opciones;  // Opciones din�micas para mostrar en pantalla
    bool m_flagMenu = true;               // Indicador para mantener el men� abierto o cerrado
};

#endif

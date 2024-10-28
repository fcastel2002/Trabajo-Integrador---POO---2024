#include "Cliente.h"
#include "PantallaCurses.h"
#include "MainMenu.h"
#include <iostream>

int main() {
    // Inicializamos los componentes principales
/*
    std::string ip = "127.0.0.1";   // Dirección IP del servidor
    int puerto = 9000;              // Puerto del servidor XMLRPC
    CLIMessageView console;
*/  

    try {
        // Inicializar la pantalla de usuario
        PantallaCurses pantalla;

        // Solicitar datos de conexión al usuario
        std::string ip = pantalla.capturarEntrada("Ingrese la IP del servidor:");
        std::string puertoStr = pantalla.capturarEntrada("Ingrese el puerto del servidor:");
        int puerto = std::stoi(puertoStr);
		std::string user = pantalla.capturarEntrada("Ingrese el usuario:");
		std::string pass = pantalla.capturarEntrada("Ingrese la clave:");

        // Crear la instancia del cliente, usando la pantalla para mensajes y errores
        Cliente cliente(ip, puerto, pantalla);
		cliente.setUser(user);
		cliente.setPass(pass); 

        // Crear la instancia de MainMenu
        MainMenu menu(cliente, &pantalla);

        // Ejecutar el menú principal
        menu.mostrarMenu();

    }
    catch (const std::exception& e) {
        // Capturamos y mostramos cualquier error que ocurra durante la ejecución
    }

    return 0;
}


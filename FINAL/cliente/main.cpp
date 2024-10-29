#include "Cliente.h"
#include "PantallaCurses.h"
#include "MainMenu.h"
#include "ErrorHandler.h"
#include <iostream>
#include <exception>

int main() {
    ErrorHandler errorHandler;  // Crea una instancia de ErrorHandler para manejar errores

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

        // Ejecutar el menú principal con manejo de errores
        menu.mostrarMenu(errorHandler);

    }
    catch (const std::exception& e) {
        // Capturamos y mostramos cualquier error que ocurra durante la ejecución
        errorHandler.handleException(e);
        errorHandler.displayError("Error crítico en la ejecución del programa.", ErrorLevel::ERROR);
    }

    return 0;
}
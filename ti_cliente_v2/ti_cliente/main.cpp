#include "Cliente.h"
#include "MainMenu.h"
#include "CLIMessageView.h"
#include <iostream>

int main() {
    // Inicializamos los componentes principales
    std::string ip = "127.0.0.1";   // Dirección IP del servidor
    int puerto = 9000;              // Puerto del servidor XMLRPC

    try {
        // Creamos la instancia del cliente
        Cliente cliente(ip, puerto);

        // Creamos el menú principal
        MainMenu menu(cliente);

        // Iniciamos el menú
        menu.mostrarMenu();

    }
    catch (const std::exception& e) {
        // Capturamos y mostramos cualquier error que ocurra durante la ejecución
        CLIMessageView view;
        view.mostrarError("Ocurrió un error inesperado: " + std::string(e.what()));
    }

    return 0;
}

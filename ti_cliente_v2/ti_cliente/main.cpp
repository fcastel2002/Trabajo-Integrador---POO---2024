#include "Cliente.h"
#include "MainMenu.h"
#include "CLIMessageView.h"
#include <iostream>

int main() {
    // Inicializamos los componentes principales
    std::string ip = "127.0.0.1";   // Direcci�n IP del servidor
    int puerto = 9000;              // Puerto del servidor XMLRPC

    try {
        // Creamos la instancia del cliente
        Cliente cliente(ip, puerto);

        // Creamos el men� principal
        MainMenu menu(cliente);

        // Iniciamos el men�
        menu.mostrarMenu();

    }
    catch (const std::exception& e) {
        // Capturamos y mostramos cualquier error que ocurra durante la ejecuci�n
        CLIMessageView view;
        view.mostrarError("Ocurri� un error inesperado: " + std::string(e.what()));
    }

    return 0;
}

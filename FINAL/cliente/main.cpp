#include "Cliente.h"
#include "PantallaCurses.h"
#include "MainMenu.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "XmlRpc.h"

bool verificarServidor(const std::string& ip, int puerto) {
    try {
        XmlRpc::XmlRpcClient client(ip.c_str(), puerto);
        XmlRpc::XmlRpcValue noArgs, result;
        if (client.execute("system.listMethods", noArgs, result)) {
            return true;
        }
    }
    catch (const XmlRpc::XmlRpcException& e) {
        return false;
    }
    return false;
}

int main() {
    try {
        PantallaCurses pantalla;
        std::string ip;
        int puerto;

        while (true) {
            ip = pantalla.capturarEntrada("Ingrese la IP del servidor:");
            std::string puertoStr = pantalla.capturarEntrada("Ingrese el puerto del servidor:");
            puerto = std::stoi(puertoStr);

            if (verificarServidor(ip, puerto)) {
                break;
            }
            else {
                pantalla.mostrarTexto("Error al conectar con el servidor\n El servidor esta apagado o los parametros son incorrectos, intente nuevamente");
            }
        }

        Cliente cliente(ip, puerto, pantalla);
        MainMenu menu(cliente, &pantalla);
        menu.mostrarMenu();
    }
    catch (const std::exception& e) {
        // Capturamos y mostramos cualquier error que ocurra durante la ejecución
    }

    return 0;
}

#include "Cliente.h"
#include <iostream>

int main() {
    std::string serverUrl = "http://127.0.0.1:9000/RPC2";
    Cliente cliente(serverUrl);

    std::string usuario = "admin";
    std::string clave = "clave123";

    if (cliente.conectar(usuario, clave)) {
        std::cout << "Conectado al servidor exitosamente." << std::endl;
    }
    else {
        std::cerr << "Error al conectar al servidor." << std::endl;
    }

    if (cliente.desconectar(usuario, clave)) {
        std::cout << "Desconectado del servidor exitosamente." << std::endl;
    }
    else {
        std::cerr << "Error al desconectar del servidor." << std::endl;
    }

    return 0;
}

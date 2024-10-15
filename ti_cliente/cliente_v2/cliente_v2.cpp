// cliente_v2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// src/cliente.cpp
#include <iostream>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client_simple.hpp>
int main() {
    try {
        xmlrpc_c::clientSimple client;
        xmlrpc_c::value result;

        std::string serverUrl = "http://127.0.0.1:9000";
        std::string methodName = "conectar";

        // Llamar al método "conectar" del servidor
        client.call(serverUrl, methodName, "", &result);

        // Convertir y mostrar el resultado
        bool success = xmlrpc_c::value_boolean(result);
        if (success) {
            std::cout << "Conexión exitosa al robot." << std::endl;
        }
        else {
            std::cout << "Error al conectar al robot." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error de XML-RPC: " << e.what() << std::endl;
    }
    return 0;
}

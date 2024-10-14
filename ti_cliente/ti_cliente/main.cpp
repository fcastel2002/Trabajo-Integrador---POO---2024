// ti_cliente.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Cliente.h"
#include <iostream>
#include <csignal>

// Global Cliente object
Cliente* cliente_ptr = nullptr;

// Signal handler for keyboard interrupt
void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received.\n";
    if (cliente_ptr) {
        cliente_ptr->desconectar();
    }
    exit(signum);
}

int main() {
    // Replace with the actual IP and port of your XML-RPC server
    std::string server_ip = "127.0.0.1";
    int server_port = 8080;

    // Create a Cliente object
    Cliente cliente(server_ip, server_port);
    cliente_ptr = &cliente; // Assign to global pointer

    // Register signal handler
    signal(SIGINT, signalHandler);

    // Test the connection
    cliente.conectar();

    // Confirm the connection
    std::string response = cliente.enviarComando("confirmar_conexion");
    std::cout << "Server response: " << response << std::endl;

    // Keep the program running to test the interrupt
    std::cout << "Press Ctrl+C to disconnect and exit the program.\n";
    while (true) {
        // Simulate work
    }

    return 0;
}

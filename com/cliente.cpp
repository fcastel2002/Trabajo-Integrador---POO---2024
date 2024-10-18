//---------------------------------------------------------------- COMPILAR CON:   g++ -I./json-develop/single_include -o cliente cliente.cpp -lws2_32

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>  // Para trabajar con JSON
#include <winsock2.h>  // API de sockets en Windows

#pragma comment(lib, "Ws2_32.lib")  // Vincular la biblioteca de Winsock

using json = nlohmann::json;
using namespace std;

#define PORT 65432
#define SERVER_ADDRESS "127.0.0.1"

int main() {
    // Inicializar Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error al inicializar Winsock" << endl;
        return -1;
    }

    // Crear el socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Error al crear el socket" << endl;
        WSACleanup();
        return -1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    // Conectar con el servidor
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        cerr << "Conexión fallida" << endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Crear un objeto JSON
    json objeto_json = {
        {"nombre", "Juan"},
        {"edad", 30},
        {"ocupacion", "Ingeniero"}
    };

    // Serializar el objeto JSON a string y enviarlo
    string mensaje = objeto_json.dump();
    send(sock, mensaje.c_str(), mensaje.length(), 0);
    cout << "Mensaje enviado: " << mensaje << endl;

    // Recibir la respuesta del servidor
    char buffer[1024] = {0};
    int valread = recv(sock, buffer, 1024, 0);
    cout << "Respuesta del servidor: " << string(buffer, valread) << endl;

    // Cerrar el socket y limpiar Winsock
    closesocket(sock);
    WSACleanup();

    return 0;
}
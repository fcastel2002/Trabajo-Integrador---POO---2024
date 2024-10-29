#pragma once

#include <string>
#include "IPantalla.h"
#include <XmlRpc.h>
#include "Orden.h"

// Librerías necesarias para XmlRpc en Visual Studio
#pragma comment(lib, "xmlrpc++.lib")
#pragma comment(lib, "WS2_32.lib")

using namespace XmlRpc;

class Cliente {
private:
    std::string m_ip;               // Dirección IP del servidor
    int m_puerto;                   // Puerto de conexión
    XmlRpcClient client;            // Cliente XML-RPC para comunicaciones con el servidor
    IPantalla& m_pantalla;          // Referencia a la interfaz de pantalla
    std::string m_usuario;          // Nombre de usuario
    std::string m_clave;            // Contraseña

public:
    // Constructor: inicializa la conexión con IP, puerto y pantalla
    Cliente(std::string ip, int puerto, IPantalla& pantalla);

    // Envia un comando al servidor a través de XML-RPC
    bool enviarComando(Orden& my_order);

    // Solicita y obtiene una lista de comandos del servidor
    std::vector<std::string> pedirComandos(Orden& my_order);

    // Interpreta la respuesta recibida del servidor
    void interpretarRespuesta(XmlRpcValue& respuesta);

    // Obtiene el nombre de usuario
    std::string getUser() { return m_usuario; }

    // Obtiene la contraseña del usuario
    std::string getPass() { return m_clave; }

    // Define el nombre de usuario
    void setUser(std::string user) { m_usuario = user; }

    // Define la contraseña del usuario
    void setPass(std::string pass) { m_clave = pass; }

    // Devuelve la referencia de la pantalla
    IPantalla& getPantalla() { return m_pantalla; }

    // Realiza el proceso de login solicitando usuario y contraseña al usuario
    void login();
};

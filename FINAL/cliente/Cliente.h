#pragma once

#include <string>
#include <vector>
#include "IPantalla.h"
#include <XmlRpc.h>
#include "Orden.h"
#include "ErrorHandler.h"

#pragma comment(lib, "xmlrpc++.lib")
#pragma comment(lib, "WS2_32.lib")	

using namespace XmlRpc;

class Cliente {
private:
    std::string m_ip;                   // Dirección IP del servidor
    int m_puerto;                       // Puerto de conexión
    XmlRpcClient client;                // Cliente XML-RPC para la comunicación con el servidor
    IPantalla& m_pantalla;              // Referencia a la pantalla para mostrar mensajes
    std::string m_usuario;              // Nombre de usuario
    std::string m_clave;                // Contraseña

    // Método auxiliar para extraer el contenido entre las etiquetas <value> y </value>
    std::string extraerContenido(XmlRpcValue& contenido);

public:
    // Constructor que inicializa la conexión con IP, puerto y pantalla
    Cliente(IPantalla& pantalla);

    // Envía un comando al servidor a través de XML-RPC
    bool enviarComando(Orden& my_order);

    // Solicita y obtiene una lista de comandos del servidor
    std::vector<std::string> pedirComandos(Orden& my_order);

    // Interpreta la respuesta recibida del servidor y muestra el mensaje limpio
    void interpretarRespuesta(XmlRpcValue& respuesta);

    // Realiza el proceso de login solicitando usuario y contraseña al usuario
    void login();
	
    // Resetea usuario y contraseña
    
	void cerrarSesion();


    bool verificarServidor(); 
    void capturarIpYPuerto();

    // Métodos getter y setter para el usuario y la contraseña
    std::string getUser() { return m_usuario; }
    std::string getPass() { return m_clave; }
    void setUser(std::string user) { m_usuario = user; }
    void setPass(std::string pass) { m_clave = pass; }

    // Devuelve la referencia de la pantalla
    IPantalla& getPantalla() { return m_pantalla; }
};

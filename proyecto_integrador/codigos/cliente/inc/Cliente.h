#pragma once

#include <string>
#include <vector>
#include "IPantalla.h"
#include <XmlRpc.h>
#include "Orden.h"
#include "ErrorHandler.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>

//#pragma comment(lib, "xmlrpc++.lib")
//#pragma comment(lib, "WS2_32.lib")	

using namespace XmlRpc;

class Cliente {
private:
    std::string m_ip;                   // Direcci�n IP del servidor
    int m_puerto;                       // Puerto de conexi�n
    XmlRpcClient client;                // Cliente XML-RPC para la comunicaci�n con el servidor
    IPantalla& m_pantalla;              // Referencia a la pantalla para mostrar mensajes
    std::string m_usuario;              // Nombre de usuario
    std::string m_clave;                // Contrase�a

    // M�todo auxiliar para extraer el contenido entre las etiquetas <value> y </value>
    std::string extraerContenido(XmlRpcValue& contenido);

public:
    // Constructor que inicializa la conexi�n con IP, puerto y pantalla
    Cliente(IPantalla& pantalla);

    // Env�a un comando al servidor a trav�s de XML-RPC
    bool enviarComando(Orden& my_order);

    // Solicita y obtiene una lista de comandos del servidor
    std::vector<std::string> pedirComandos(Orden& my_order);

    // Interpreta la respuesta recibida del servidor y muestra el mensaje limpio
    void interpretarRespuesta(XmlRpcValue& respuesta);

    // Realiza el proceso de login solicitando usuario y contrase�a al usuario
    void login();
	
    // Resetea usuario y contrase�a
    
	void cerrarSesion();


    bool verificarServidor(); 
    void capturarIpYPuerto();

    // M�todos getter y setter para el usuario y la contrase�a
    std::string getUser() { return m_usuario; }
    std::string getPass() { return m_clave; }
    void setUser(std::string user) { m_usuario = user; }
    void setPass(std::string pass) { m_clave = pass; }

    // Devuelve la referencia de la pantalla
    IPantalla& getPantalla() { return m_pantalla; }
};

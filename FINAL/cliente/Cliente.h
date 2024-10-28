#pragma once

#include <string>
#include "IPantalla.h"
#include <XmlRpc.h>
#include "Orden.h"

#pragma comment(lib, "xmlrpc++.lib")
#pragma comment(lib, "WS2_32.lib")	


using namespace XmlRpc;

class Cliente {

private:
	std::string m_ip;
	int m_puerto;
	XmlRpcClient client;
	IPantalla& m_pantalla;
	std::string m_usuario;
	std::string m_clave;
public:

	Cliente(std::string ip, int puerto, IPantalla& pantalla);

	bool enviarComando(Orden& my_order);
	std::vector<std::string> pedirComandos(Orden& my_order);

	void interpretarRespuesta(XmlRpcValue& respuesta);

	std::string getUser() { return m_usuario; }
	std::string getPass() { return m_clave; }
	void setUser(std::string user) { m_usuario = user; }

	IPantalla& getPantalla() { return m_pantalla; }

	void setPass(std::string pass) { m_clave = pass; }
//const std::vector<std::string> pedirComandos(Orden& my_order);
};


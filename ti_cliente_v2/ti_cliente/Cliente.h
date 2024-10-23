#pragma once

#include <string>
#include "CLIMessageView.h"
#include <XmlRpc.h>
#include "Orden.h"
#include "nlohmann/json.hpp"

#pragma comment(lib, "xmlrpc++.lib")
#pragma comment(lib, "WS2_32.lib")	


using namespace XmlRpc;

class Cliente {

private:
	std::string m_ip;
	int m_puerto;
	XmlRpcClient client;
	CLIMessageView& m_console;
	std::string m_usuario;
	std::string m_clave;
public:

	Cliente(std::string ip, int puerto, CLIMessageView& console);

	bool enviarComando(const Orden& my_order);
	std::vector<std::string> pedirComandos(Orden& my_order);

	void interpretarRespuesta(std::string& respuesta);

	std::string getUser() { return m_usuario; }
	std::string getPass() { return m_clave; }
	void setUser(std::string user) { m_usuario = user; }
	void setPass(std::string pass) { m_clave = pass; }
    CLIMessageView& getConsole() { return m_console; }
	//const std::vector<std::string> pedirComandos(Orden& my_order);
};


#pragma once

#include <string>
#include <XmlRpc.h>
#pragma comment(lib, "xmlrpc++.lib")
#pragma comment(lib, "WS2_32.lib")	


using namespace XmlRpc;

class Cliente {

private:
	std::string m_ip;
	int m_puerto;
public:

	Cliente(std::string ip, int puerto);
	
	bool conectarServidor();
	bool enviarComando(const std::string& comando);

};
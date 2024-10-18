#pragma once

#include <string>
#include "CLIMessageView.h"
#include <XmlRpc.h>
#pragma comment(lib, "xmlrpc++.lib")
#pragma comment(lib, "WS2_32.lib")	


using namespace XmlRpc;

class Cliente {

private:
	std::string m_ip;
	int m_puerto;
	XmlRpcClient client;
public:

	Cliente(std::string ip, int puerto);
	
	bool conectarServidor();
	bool enviarComando(const std::string& comando);
	bool desconectarMotores();

};
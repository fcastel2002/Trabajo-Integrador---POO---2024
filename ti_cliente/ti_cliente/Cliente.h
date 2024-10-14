#pragma once
#include <string>
#include "../XMLRPC++/XmlRpc.h"
#pragma comment(lib, "WS2_32.lib")
#include <iostream>
#pragma comment(lib, "./x64/Debug/xmlrpc++.lib")
using namespace XmlRpc;
class Cliente
{
private:
	
	std::string m_ip{};
	int m_port{};
	bool connected{ false };
	XmlRpcClient client;

public:

	Cliente(std::string ip, int port);
	void conectar();
	void desconectar();
	std::string enviarComando(std::string comando, XmlRpcValue params);

};


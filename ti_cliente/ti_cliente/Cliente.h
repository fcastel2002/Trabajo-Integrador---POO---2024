#pragma once
#include <string>
#include "../XMLRPC++/XmlRpc.h"
#pragma comment(lib, "WS2_32.lib")
#include <iostream>
#include "IMessageView.h"
#pragma comment(lib, "./x64/Debug/xmlrpc++.lib")
using namespace XmlRpc;
class Cliente
{
private:
	
	std::string m_ip{};
	int m_port{};
	bool connected{ false };
	XmlRpcClient *client;
	IMessageView& m_messageView;

public:

	Cliente(std::string ip, int port, IMessageView& messageView);
	~Cliente();
	bool enviarComando(const std::string& comando, XmlRpcValue& result);
	bool enviarComando(const std::string& comando);
};


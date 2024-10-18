#include "Cliente.h"


Cliente::Cliente(std::string ip, int puerto, CLIMessageView& console) : m_ip(ip), m_puerto(puerto), client(m_ip.c_str(), m_puerto), m_console{console} {}


bool Cliente::enviarComando(const std::string& comando) {
	XmlRpcValue params, result;
	params[0] = "admin";
	params[1] = "clave123";

	client.execute(comando.c_str(), params, result);
	client.close();
	return true;
}


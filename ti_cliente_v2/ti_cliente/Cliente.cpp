#include "Cliente.h"


Cliente::Cliente(std::string ip, int puerto) : m_ip(ip), m_puerto(puerto) {}


bool Cliente::conectarServidor() {
	XmlRpcClient client(m_ip.c_str(), m_puerto);
	XmlRpcValue noArgs, result;
	client.execute("listar_comandos", noArgs, result);
	return true;
}

bool Cliente::enviarComando(const std::string& comando) {
	return true;
}
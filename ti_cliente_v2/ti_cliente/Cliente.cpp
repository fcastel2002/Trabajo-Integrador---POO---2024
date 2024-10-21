#include "Cliente.h"


Cliente::Cliente(std::string ip, int puerto, CLIMessageView& console) 
	: m_ip(ip)
	, m_puerto(puerto)
	, client(m_ip.c_str()
	, m_puerto)
	, m_console{console}
	, m_usuario{ "admin" }
	, m_clave{"clave123"} {}


bool Cliente::enviarComando(const Orden& my_order) {
	XmlRpcValue params, result;
	std::string cmd;
	cmd = my_order.toJson(m_usuario, m_clave);
	params[0] = cmd;
	
	client.execute("Interpreta_Comando", params, result);
	interpretarRespuesta(result);
	client.close();
	return true;
}

const std::string Cliente::interpretarRespuesta(std::string& respuesta) {
	m_console.mostrarRespuesta(respuesta); 
	return "";
}
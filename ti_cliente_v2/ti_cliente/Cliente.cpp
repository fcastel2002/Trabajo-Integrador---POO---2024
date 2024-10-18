#include "Cliente.h"


Cliente::Cliente(std::string ip, int puerto) : m_ip(ip), m_puerto(puerto), client(m_ip.c_str(), m_puerto){}


bool Cliente::conectarServidor() {
	XmlRpcValue params, result;
	params[0] = "admin";
	params[1] = "clave123";
	client.execute("activar_motores", params, result);
	CLIMessageView mensaje;
	mensaje.mostrarMensaje(result);
	client.close();
	return true;
}

bool Cliente::enviarComando(const std::string& comando) {
	return true;
}

bool Cliente::desconectarMotores() {

	XmlRpcValue params, result;
	params[0] = "admin";
	params[1] = "clave123";

	client.execute("desactivar_motores", params, result);
	CLIMessageView mensaje;
	mensaje.mostrarMensaje(result);
	client.close();
	return true;
	
}
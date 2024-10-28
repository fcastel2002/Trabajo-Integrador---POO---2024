#include "Cliente.h"
#include "ErrorHandler.h"
#include <iostream>

Cliente::Cliente(std::string ip, int puerto, IPantalla& pantalla) 
	: m_ip(ip)
	, m_puerto(puerto)
	, client(m_ip.c_str(), m_puerto)
	, m_pantalla{pantalla}
	, m_usuario{ "null" }
	, m_clave{"null"} {}

bool Cliente::enviarComando(Orden& my_order, ErrorHandler& errorHandler) {
	XmlRpcValue params, result;
	params = my_order.crearOrden(m_usuario, m_clave);

	try {
		client.execute("Interpreta_Comando", params, result);
	} catch (const std::exception& e) {
		errorHandler.handleException(e);
		client.close();
		return false;
	}
	
	interpretarRespuesta(result, errorHandler);
	client.close();
	return true;
}

void Cliente::interpretarRespuesta(XmlRpcValue& respuesta, ErrorHandler& errorHandler) {
	try {
		m_pantalla.mostrarTexto(respuesta.toXml());
	} catch (const std::exception& e) {
		errorHandler.handleException(e);
	}
}

std::vector<std::string> Cliente::pedirComandos(Orden& my_order, ErrorHandler& errorHandler) {
	XmlRpcValue params, result;
	params = my_order.crearOrden(m_usuario, m_clave);

	try {
		client.execute("Interpreta_Comando", params, result);
	} catch (const std::exception& e) {
		errorHandler.handleException(e);
		client.close();
		return {};
	}

	std::vector<std::string> comandos;
	if (result.getType() == XmlRpcValue::TypeArray) {
		for (int i = 0; i < result.size(); ++i) {
			comandos.push_back(result[i]);
		}
	}

	client.close();
	return comandos;
}
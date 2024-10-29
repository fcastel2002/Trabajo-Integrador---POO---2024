#include "Cliente.h"
#include "ErrorHandler.h"
#include <iostream>

Cliente::Cliente(std::string ip, int puerto, IPantalla& pantalla)
	: m_ip(ip)
	, m_puerto(puerto)
	, client(m_ip.c_str(), m_puerto)
	, m_pantalla{ pantalla }
	, m_usuario{ "null" }
	, m_clave{ "null" } {}

bool Cliente::enviarComando(Orden& my_order) {
	XmlRpcValue params, result;
	params = my_order.crearOrden();

	ErrorHandler errorHandler;

	try {
		if (!client.execute("Interpreta_Comando", params, result)) {
			errorHandler.logError(ErrorCode::CONNECTION_FAILED, ErrorLevel::ERROR);
			errorHandler.displayError("Error al enviar el comando al servidor.", ErrorLevel::ERROR);
			return false;
		}

		interpretarRespuesta(result);
		client.close();
		return true;
	}
	catch (const std::exception& e) {
		errorHandler.handleException(e);
		client.close();
		return false;
	}
}

void Cliente::interpretarRespuesta(XmlRpcValue& respuesta) {
	ErrorHandler errorHandler;
	if (respuesta.getType() != XmlRpcValue::TypeString) {
		errorHandler.logError("Respuesta no valida recibida del servidor.", ErrorLevel::WARNING);
		errorHandler.displayError("La respuesta del servidor no tiene el formato esperado.", ErrorLevel::WARNING);
		return;
	}

	m_pantalla.mostrarTexto(respuesta.toXml());
}

std::vector<std::string> Cliente::pedirComandos(Orden& my_order) {
	XmlRpcValue params, result;
	std::vector<std::string> comandos;
	ErrorHandler errorHandler;

	params = my_order.crearOrden();

	try {
		if (!client.execute("Interpreta_Comando", params, result)) {
			errorHandler.logError(ErrorCode::CONNECTION_FAILED, ErrorLevel::ERROR);
			errorHandler.displayError("Error al solicitar comandos del servidor.", ErrorLevel::ERROR);
			client.close();
			return comandos;
		}

		if (result.getType() == XmlRpcValue::TypeArray) {
			for (int i = 0; i < result.size(); ++i) {
				if (result[i].getType() == XmlRpcValue::TypeString) {
					comandos.push_back(result[i]);
				}
			}
		}
		else {
			errorHandler.logError("Formato de respuesta incorrecto.", ErrorLevel::WARNING);
			errorHandler.displayError("El servidor devolvio un formato no esperado.", ErrorLevel::WARNING);
		}

		client.close();
		return comandos;
	}
	catch (const std::exception& e) {
		errorHandler.handleException(e);
		client.close();
		return comandos;
	}
}

void Cliente::login() {
	std::string user = m_pantalla.capturarEntrada("Ingrese su usuario: ");
	std::string pass = m_pantalla.capturarEntrada("Ingrese su clave: ");
	setUser(user);
	setPass(pass);
}

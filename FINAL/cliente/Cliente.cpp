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

	// Limpia el mensaje recibido eliminando etiquetas XML
	std::string mensajeLimpio = extraerContenido(respuesta.toXml());
	m_pantalla.mostrarTexto(mensajeLimpio);
}

// Método auxiliar para extraer el contenido entre las etiquetas <value> y </value>
std::string Cliente::extraerContenido(const std::string& mensaje) {
	std::size_t start = mensaje.find("<value>");
	std::size_t end = mensaje.find("</value>");

	if (start != std::string::npos && end != std::string::npos) {
		start += 7;  // Mueve el índice justo después de "<value>"
		return mensaje.substr(start, end - start);  // Extrae el contenido entre las etiquetas
	}

	// Si no se encuentran las etiquetas, retorna el mensaje completo
	return mensaje;
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

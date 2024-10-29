#include "Cliente.h"
#include "ErrorHandler.h"
#include <iostream>
#include <unordered_map>

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
		errorHandler.logError("Respuesta no válida recibida del servidor.", ErrorLevel::WARNING);
		errorHandler.displayError("La respuesta del servidor no tiene el formato esperado.", ErrorLevel::WARNING);
		return;
	}

	// Limpia el mensaje recibido eliminando etiquetas XML y reemplazando entidades HTML
	std::string mensajeLimpio = extraerContenido(respuesta.toXml());
	m_pantalla.mostrarTexto(mensajeLimpio);
}

// Método auxiliar para extraer el contenido entre las etiquetas <value> y </value> y limpiar entidades HTML
std::string Cliente::extraerContenido(const std::string& mensaje) {
	std::size_t start = mensaje.find("<value>");
	std::size_t end = mensaje.find("</value>");

	std::string contenido;
	if (start != std::string::npos && end != std::string::npos) {
		start += 7;  // Mueve el índice justo después de "<value>"
		contenido = mensaje.substr(start, end - start);  // Extrae el contenido entre las etiquetas
	}
	else {
		contenido = mensaje;
	}

	// Reemplaza las entidades HTML comunes con sus caracteres equivalentes
	return reemplazarEntidadesHTML(contenido);
}

// Método auxiliar para reemplazar entidades HTML comunes
std::string Cliente::reemplazarEntidadesHTML(const std::string& texto) {
	std::string limpio = texto;
	const std::unordered_map<std::string, std::string> entidades = {
		{"&apos;", "'"},
		{"&quot;", "\""},
		{"&lt;", "<"},
		{"&gt;", ">"},
		{"&amp;", "&"}
	};

	for (const auto& [entidad, caracter] : entidades) {
		std::size_t pos = limpio.find(entidad);
		while (pos != std::string::npos) {
			limpio.replace(pos, entidad.length(), caracter);
			pos = limpio.find(entidad, pos + caracter.length());
		}
	}
	return limpio;
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
			errorHandler.displayError("El servidor devolvió un formato no esperado.", ErrorLevel::WARNING);
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

// Implementación de la función login
void Cliente::login() {
	std::string user = m_pantalla.capturarEntrada("Ingrese su usuario: ");
	std::string pass = m_pantalla.capturarEntrada("Ingrese su clave: ");
	setUser(user);
	setPass(pass);
}

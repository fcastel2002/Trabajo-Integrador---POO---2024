#include "Cliente.h"

#include "RespuestaBuilder.h"
#include "ErrorHandler.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>

/**
 * Constructor de la clase Cliente.
 * @param pantalla Referencia a una interfaz de pantalla para la interacción con el usuario.
 */
Cliente::Cliente(IPantalla& pantalla)
	: m_pantalla{ pantalla }
	, m_usuario{ "null" }
	, m_clave{ "null" }
	, client("", 0) {
	capturarIpYPuerto();
}

/**
 * Captura la IP y el puerto del servidor desde la entrada del usuario.
 */
void Cliente::capturarIpYPuerto() {
	m_ip = m_pantalla.capturarEntrada("Ingrese la IP del servidor: ");
	std::string puertoStr = m_pantalla.capturarEntrada("Ingrese el puerto del servidor: ");
	try {
		m_puerto = std::stoi(puertoStr);
	}
	catch (const std::invalid_argument& e) {
		ErrorHandler errorHandler;
		errorHandler.logError("El puerto ingresado no es un numero valido.", ErrorLevel::ERROR);
		errorHandler.displayError("El puerto ingresado no es un numero valido.", ErrorLevel::ERROR);
		capturarIpYPuerto();
	}
	verificarServidor();
}

/**
 * Verifica la conexión con el servidor.
 * @return true si la conexión es exitosa, false en caso contrario.
 */
bool Cliente::verificarServidor() {
	ErrorHandler errorHandler;
	client = XmlRpcClient(m_ip.c_str(), m_puerto);
	while (true) {
		try {
			XmlRpcValue noArgs, result;

			if (client.execute("system.listMethods", noArgs, result)) {
				client.close();
				return true;
			}
			else {
				errorHandler.logError(ErrorCode::CONNECTION_FAILED, ErrorLevel::ERROR);
				errorHandler.displayError("Conexion fallida con el servidor\n Servidor apagado o parametros erroneos.", ErrorLevel::ERROR);
				capturarIpYPuerto();
			}
		}
		catch (const XmlRpcException& e) {
			errorHandler.logError(e.getMessage(), ErrorLevel::ERROR);
			errorHandler.displayError("Conexion fallida con el servidor\nServidor apagado o parametros erroneos.", ErrorLevel::ERROR);
			capturarIpYPuerto();
		}
	}
	return false;
}

/**
 * Envía un comando al servidor.
 * @param my_order Referencia a un objeto Orden que contiene el comando a enviar.
 * @return true si el comando se envía correctamente, false en caso contrario.
 */
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

/**
 * Interpreta la respuesta recibida del servidor.
 * @param respuesta Referencia a un objeto XmlRpcValue que contiene la respuesta del servidor.
 */
void Cliente::interpretarRespuesta(XmlRpcValue& respuesta) {
	ErrorHandler errorHandler;
	if (respuesta.getType() != XmlRpcValue::TypeArray) {
		errorHandler.logError("Respuesta no válida recibida del servidor.", ErrorLevel::WARNING);
		errorHandler.displayError("La respuesta del servidor no tiene el formato esperado.", ErrorLevel::WARNING);
		return;
	}

	// Limpia el mensaje recibido eliminando etiquetas XML y reemplazando entidades HTML
	RespuestaBuilder builder;
	builder.conUsuario(respuesta[0])
		.conComando(respuesta[1])
		.conContenido(extraerContenido(respuesta[2]));

	Respuesta currentRespuesta = builder.build();
	m_pantalla.mostrarTexto(currentRespuesta.getContenido());
}

/**
 * Extrae el contenido de un objeto XmlRpcValue.
 * @param contenido Referencia a un objeto XmlRpcValue que contiene el contenido a extraer.
 * @return Una cadena de texto con el contenido extraído.
 */
std::string Cliente::extraerContenido(XmlRpcValue& contenido) {
	if (contenido.getType() != XmlRpcValue::TypeArray) {
		return "";
	}

	std::string resultado;
	for (int i = 0; i < contenido.size(); ++i) {
		if (contenido[i].getType() == XmlRpcValue::TypeString) {
			resultado += static_cast <std::string>(contenido[i]) + "\n";
		}
	}

	// Elimina el último salto de línea si existe
	if (!resultado.empty()) {
		resultado.pop_back();
	}

	return resultado;
}

/**
 * Solicita comandos del servidor.
 * @param my_order Referencia a un objeto Orden que contiene la solicitud de comandos.
 * @return Un vector de cadenas de texto con los comandos recibidos.
 */
std::vector<std::string> Cliente::pedirComandos(Orden& my_order) {
	XmlRpcValue params, result;
	std::vector<std::string> comandos;
	ErrorHandler errorHandler;

	params = my_order.crearOrden();

	try {
		if (!client.execute("Interpreta_Comando", params, result)) {
			errorHandler.logError(ErrorCode::CONNECTION_FAILED, ErrorLevel::ERROR);
			errorHandler.displayError("Error al solicitar comandos del servidor.\n\n\n\n Revise usuario y clave", ErrorLevel::ERROR);
			client.close();
			return comandos;
		}

		// Verifica que la respuesta es un array
		if (result.getType() == XmlRpcValue::TypeArray) {
			// Verifica que el tercer elemento de la respuesta es un array
			if (result.size() > 2 && result[2].getType() == XmlRpcValue::TypeArray) {
				for (int i = 0; i < result[2].size(); ++i) {
					if (result[2][i].getType() == XmlRpcValue::TypeString) {
						comandos.push_back(result[2][i]);
					}
				}
			}
			else {
				errorHandler.logError("El tercer elemento de la respuesta no es un array.", ErrorLevel::WARNING);
				errorHandler.displayError("El formato del tercer elemento de la respuesta no es el esperado.", ErrorLevel::WARNING);
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

/**
 * Realiza el proceso de inicio de sesión del usuario.
 */
void Cliente::login() {
	std::string user = m_pantalla.capturarEntrada("Ingrese su usuario: ");
	std::string pass = m_pantalla.capturarEntrada("Ingrese su clave: ");
	setUser(user);
	setPass(pass);
}

/**
 * Cierra la sesión del usuario.
 */
void Cliente::cerrarSesion() {
	setUser("null");
	setPass("null");
}

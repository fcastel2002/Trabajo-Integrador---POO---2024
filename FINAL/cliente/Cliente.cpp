#include "Cliente.h"

#include "RespuestaBuilder.h"
#include "ErrorHandler.h"
#include <iostream>
#include <unordered_map>

Cliente::Cliente(IPantalla& pantalla)
	: m_pantalla{ pantalla }
	, m_usuario{ "null" }
	, m_clave{ "null" }
	, client("",0){
	capturarIpYPuerto();
	
	}

void Cliente::capturarIpYPuerto() {
	m_ip = m_pantalla.capturarEntrada("Ingrese la IP del servidor: ");
	std::string puertoStr = m_pantalla.capturarEntrada("Ingrese el puerto del servidor: ");
	m_puerto = std::stoi(puertoStr);
	verificarServidor();
}

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
	 
	//std::string mensajeLimpio = extraerContenido(respuesta.toXml()); // contenido del mensaje
	Respuesta currentRespuesta = builder.build();
	 // contenido del mensaje
	
	
	m_pantalla.mostrarTexto(currentRespuesta.getContenido());
}

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

	// Elimina el último salto de línea si existeA
	if (!resultado.empty() && resultado.back() == '\n') {
		resultado.pop_back();
	}

	return resultado;
}




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

// Implementación de la función login
void Cliente::login() {
	std::string user = m_pantalla.capturarEntrada("Ingrese su usuario: ");
	std::string pass = m_pantalla.capturarEntrada("Ingrese su clave: ");
	setUser(user);
	setPass(pass);
}

void Cliente::cerrarSesion() {

	setUser("null");
	setPass("null");

}
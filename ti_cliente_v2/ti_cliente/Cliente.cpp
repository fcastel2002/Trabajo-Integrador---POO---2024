#include "Cliente.h"

#include <nlohmann/json.hpp>  

#include <iostream>

Cliente::Cliente(std::string ip, int puerto, CLIMessageView& console) 
	: m_ip(ip)
	, m_puerto(puerto)
	, client(m_ip.c_str(), m_puerto)
	, m_console{console}
	, m_usuario{ "admin" }
	, m_clave{"clave123"} {}


bool Cliente::enviarComando(Orden& my_order) {
	XmlRpcValue params, result;
	params = my_order.crearOrden(m_usuario,m_clave);
			 
	
	client.execute("Interpreta_Comando", params, result);
	interpretarRespuesta(result);
	client.close();
	return true;
}
void Cliente::interpretarRespuesta(XmlRpcValue& respuesta) {
	m_console.mostrarRespuesta(respuesta);

}


std::vector<std::string> Cliente::pedirComandos(Orden& my_order) {
	XmlRpcValue params, result;
	params = my_order.crearOrden(m_usuario, m_clave);
	client.execute("Interpreta_Comando", params, result);
	interpretarRespuesta(result);

	std::vector<std::string> comandos;
	if (result.hasMember("resultado") && result["resultado"].getType() == XmlRpcValue::TypeArray) {
		for (int i = 0; i < result["resultado"].size(); ++i) {
			if (result["resultado"][i].getType() == XmlRpcValue::TypeString) {
				comandos.push_back(result["resultado"][i]);
			}
		}
	}

	client.close();
	return comandos;
}

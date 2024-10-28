#include "Cliente.h"


#include <iostream>

Cliente::Cliente(std::string ip, int puerto, CLIMessageView& console) 
	: m_ip(ip)
	, m_puerto(puerto)
	, client(m_ip.c_str(), m_puerto)
	, m_console{console}
	, m_usuario{ "null" }
	, m_clave{"null"} {}


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
    if (result.getType() == XmlRpcValue::TypeArray) {
        for (int i = 0; i < result.size(); ++i) {
            comandos.push_back(result[i]);
        }
    }

	client.close();
	return comandos;
}

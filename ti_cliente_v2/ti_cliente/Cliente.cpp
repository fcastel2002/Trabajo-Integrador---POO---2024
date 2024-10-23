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
	m_console.mostrarMensaje(cmd); 
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


const std::vector<std::string> Cliente::pedirComandos(Orden& my_order) {
    XmlRpcValue params, result;
    std::string cmd;
    cmd = my_order.toJson(m_usuario, m_clave);
    m_console.mostrarMensaje(cmd);
    params[0] = cmd;
    client.execute("Interpreta_Comando", params, result);

    std::vector<std::string> comandos;

    // Verificar el tipo de result
    XmlRpcValue::Type resultType = result.getType();
    switch (resultType) {
    case XmlRpcValue::TypeBoolean:
        m_console.mostrarMensaje("Tipo de result: Boolean");
        break;
    case XmlRpcValue::TypeInt:
        m_console.mostrarMensaje("Tipo de result: Int");
        break;
    case XmlRpcValue::TypeDouble:
        m_console.mostrarMensaje("Tipo de result: Double");
        break;
    case XmlRpcValue::TypeString:
        m_console.mostrarMensaje("Tipo de result: String");
        m_console.mostrarMensaje(result);
        break;
    case XmlRpcValue::TypeDateTime:
        m_console.mostrarMensaje("Tipo de result: DateTime");
        break;
    case XmlRpcValue::TypeBase64:
        m_console.mostrarMensaje("Tipo de result: Base64");
        break;
    case XmlRpcValue::TypeArray:
        m_console.mostrarMensaje("Tipo de result: Array");
        break;
    case XmlRpcValue::TypeStruct:
        m_console.mostrarMensaje("Tipo de result: Struct");
        break;
    default:
        m_console.mostrarMensaje("Tipo de result: Desconocido");
        break;
    }

    return comandos;
}

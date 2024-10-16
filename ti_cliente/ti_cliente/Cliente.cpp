#include "Cliente.h"


Cliente::Cliente(std::string ip, int port, IMessageView& messageView) : m_ip{ ip }, m_port{ port }, m_messageView{ messageView } {
	client = new XmlRpcClient(m_ip.c_str(), m_port);
	

}

Cliente::~Cliente() {
	delete client;
}

bool Cliente::enviarComando(const std::string& comando, XmlRpcValue& result) {
	XmlRpcValue noArgs;

	try {
		bool requestSuccess = client->execute(comando.c_str(), noArgs, result);
		if (requestSuccess) {
			bool success = static_cast<bool>(result);
			if (success) {
				m_messageView.showMessage("Comando '" + comando + "' ejecutado correctamente", MessageType::INFO);
				return true;
			}
			else {
				m_messageView.showMessage("Error al ejecutar el comando '" + comando + "'", MessageType::ERROR);
				return false;
			}
		}
		else {
			m_messageView.showMessage("Error en la llamada XML-RPC para el comando '" + comando +"'", MessageType::ERROR);
			return false;
		}
	}
	catch (XmlRpcException& e) { //advierte de error en la llamada mediante excepcion (ojo podria estar de mas el else de arriba)
		m_messageView.showMessage("Excepción al ejecutar el comando '" + comando + "': " + e.getMessage(), MessageType::ERROR);
		return false;
	}
}

bool Cliente::enviarComando(const std::string& comando) {
	XmlRpcValue result;
	return enviarComando(comando, result);
}
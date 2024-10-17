#include "Cliente.h"

Cliente::Cliente(std::string ip, int port, IMessageView& messageView)
    : m_ip{ ip }, m_port{ port }, m_messageView{ messageView }, client(ip.c_str(), port) {
    m_messageView.showMessage("Cliente inicializado con IP: " + ip + " y puerto: " + std::to_string(port), MessageType::INFO);
}

bool Cliente::enviarComando(const std::string& comando) {
    XmlRpcValue params,result;  
    params[0] = comando;
    m_messageView.showMessage("Comando recibido: " + comando, MessageType::WARNING);
    try {
        m_messageView.showMessage("Enviando comando: " + comando, MessageType::INFO);

        // Enviar el comando al servidor (asegúrate de que el comando es el nombre correcto)
        bool requestSuccess = client.execute(comando.c_str(), comando, result);  // Usar valor vacío

        if (requestSuccess) {
            m_messageView.showMessage("Resultado recibido: " + result.toXml(), MessageType::INFO);

            // Verificar el tipo de resultado
            if (result.getType() == XmlRpcValue::TypeBoolean) {
                bool success = result;
                if (success) {
                    m_messageView.showMessage("Comando ejecutado correctamente", MessageType::INFO);
                }
                else {
                    m_messageView.showMessage("Error al ejecutar el comando", MessageType::ERROR);
                }
            }
            else {
                m_messageView.showMessage("El resultado no es un booleano: " + result.toXml(), MessageType::INFO);
            }

            return true;
        }
        else {
            m_messageView.showMessage("Error en la llamada XML-RPC para el comando '" + comando + "'", MessageType::ERROR);
            return false;
        }
    }
    catch (XmlRpcException& e) {
        m_messageView.showMessage("Excepción al ejecutar el comando: " + e.getMessage(), MessageType::ERROR);
        return false;
    }
}

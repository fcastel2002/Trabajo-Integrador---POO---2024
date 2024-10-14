#include "Cliente.h"


Cliente::Cliente(std::string ip, int port) : m_ip{ ip }, m_port{ port }, client(ip.c_str(), port) {}

void Cliente::conectar() {
    XmlRpcValue noParams, result;
    if (client.execute("conectar", noParams, result)) {
        connected = true;
        std::cout << "Connected to the robot: " << result << std::endl;
    }
    else {
        std::cerr << "Failed to connect to the robot." << std::endl;
    }
}

void Cliente::desconectar() {
    XmlRpcValue noParams, result;
    if (client.execute("desconectar", noParams, result)) {
        connected = false;
        std::cout << "Disconnected from the robot." << std::endl;
    }
    else {
        std::cerr << "Failed to disconnect from the robot." << std::endl;
    }
}

std::string Cliente::enviarComando(const std::string comando, XmlRpcValue params) {
    XmlRpcValue result;
    if (client.execute("conectar", params, result)) {
        return result.toXml();
    }
    else {
        return "Failed to execute command.";
    }
}
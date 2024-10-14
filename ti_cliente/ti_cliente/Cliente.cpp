#include "Cliente.h"


Cliente::Cliente(std::string ip, int port) : m_ip{ ip }, m_port{ port }, client(m_ip.c_str(), m_port) {}



void Cliente::conectar() {
	try {
		XmlRpcValue noArgs, result;
		client.execute("connect", noArgs, result);
        connected = result.getType() == XmlRpc::XmlRpcValue::TypeBoolean && static_cast<bool>(result);
        std::cout << "Conectado: " << connected << std::endl;
	}
	catch (XmlRpcException& e) {
		std::cout << "Error: " << e.getMessage() << std::endl;
	}
}
void Cliente::desconectar() {
    if (connected) {
        try {
            XmlRpcClient client(m_ip.c_str(), m_port);
            XmlRpcValue args, result;
            client.execute("disconnect", args, result);
            connected = false;
            std::cout << "Disconnected from " << m_ip << ":" << m_port << std::endl;
        }
        catch (const XmlRpc::XmlRpcException& e) {
            std::cerr << "Disconnection failed: " << e.getMessage() << std::endl;
        }
    }
    else {
        std::cerr << "Not connected." << std::endl;
    }
}

std::string Cliente::enviarComando(std::string comando) {
    if (connected) {
        try {
            XmlRpcValue args, result;
            args[0] = comando;
            client.execute("executeCommand", args, result);
            return result.toXml();
        }
        catch (const XmlRpc::XmlRpcException& e) {
            std::cerr << "Command execution failed: " << e.getMessage() << std::endl;
            return "";
        }
    }
    else {
        std::cerr << "Not connected." << std::endl;
        return "";
    }
}

std::string Cliente::generarNumero(double cota_inferior, double cota_superior, const std::string& alias, const std::string& ip) {
    if (connected) {
        try {
            XmlRpcValue args, result;
            args[0] = cota_inferior;
            args[1] = cota_superior;
            args[2] = alias;
            args[3] = ip;
            client.execute("generarNumero", args, result);
            if (result.getType() == XmlRpc::XmlRpcValue::TypeArray && result.size() == 2) {
                double numero = static_cast<double>(result[0]);
                std::string tiempo = static_cast<std::string>(result[1]);
                return "Numero: " + std::to_string(numero) + ", Tiempo: " + tiempo;
            }
            else {
                return result.toXml();
            }
        }
        catch (const XmlRpc::XmlRpcException& e) {
            std::cerr << "Command execution failed: " << e.getMessage() << std::endl;
            return "";
        }
    }
    else {
        std::cerr << "Not connected." << std::endl;
        return "";
    }
}

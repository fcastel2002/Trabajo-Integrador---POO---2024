#include "Cliente.h"
#include <iostream>

Cliente::Cliente(const std::string& serverUrl) : serverUrl(serverUrl), client(serverUrl.c_str(), 9000) {}

bool Cliente::conectar(const std::string& usuario, const std::string& clave) {
    XmlRpc::XmlRpcValue args, result;
    args[0] = usuario;
    args[1] = clave;
    if (client.execute("conectar", args, result)) {
        bool success = result;
        return success;
    }
    else {
        std::cerr << "Error: Failed to execute 'conectar' method." << std::endl;
        return false;
    }
}

bool Cliente::desconectar(const std::string& usuario, const std::string& clave) {
    XmlRpc::XmlRpcValue args, result;
    args[0] = usuario;
    args[1] = clave;
    if (client.execute("desconectar", args, result)) {
        bool success = result;
        return success;
    }
    else {
        std::cerr << "Error: Failed to execute 'desconectar' method." << std::endl;
        return false;
    }
}

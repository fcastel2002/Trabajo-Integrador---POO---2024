//
// Created by Pachi on 09/10/2024.
//

#include "Cliente.h"
#include <iostream>

Cliente::Cliente(std::string ipServer, int puertoServer) : m_ipServer{ipServer}, m_puertoServer{puertoServer} {
    cliente = new XmlRpc::XmlRpcClient(m_ipServer.c_str(), m_puertoServer);
    XmlRpc::XmlRpcValue resultado;
}
Cliente::~Cliente() {
    delete cliente;
}

bool Cliente::conectar() {
    conectado = true;
    return conectado;
}
void Cliente::desconectar() {
    conectado = false;
}
void Cliente::enviarArchivo(FileManager& filemanager) {
    filemanager.serializar();
}
bool Cliente::ejecutarComando(const std::string& metodo, XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& resultado) {
    try {
        bool llamadaExitosa = cliente->execute(metodo.c_str(), params, resultado);
        if (llamadaExitosa) {
            std::cout << "Resultado del servidor: " << resultado.toXml() << std::endl;
        } else {
            std::cerr << "Error: La llamada al método falló." << std::endl;
        }
        return llamadaExitosa;
    } catch (const XmlRpc::XmlRpcException& e) {
        std::cerr << "Error: " << e.getMessage() << std::endl;
        return false;
    }
}
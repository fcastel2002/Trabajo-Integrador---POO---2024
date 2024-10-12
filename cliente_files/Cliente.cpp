//
// Created by Pachi on 09/10/2024.
//

#include "Cliente.h"


Cliente::Cliente(std::string ipServer, std::string puertoServer) : m_ipServer{ipServer}, m_puertoServer{puertoServer} {}

bool Cliente::conectar() {
    conectado = true;
    return conectado;
}
void Cliente::desconectar() {
    conectado = false;
}

std::string Cliente::enviarOrden(const Orden& orden) {
    return orden.serializar();
}
void Cliente::enviarArchivo(FileManager& filemanager) {
    filemanager.serializar();
}
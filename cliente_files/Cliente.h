//
// Created by Pachi on 09/10/2024.
//

#ifndef CLIENTE_H
#define CLIENTE_H
#include <string>
#include "Orden.h"
#include "FileManager.h"

class Cliente {
private:
    std::string m_ipServer{};
    std::string m_puertoServer{};
    bool conectado = false;
public:
    Cliente(std::string ipServer, std::string puertoServer);
    bool conectar();
    void desconectar();
    std::string enviarOrden(const Orden& orden);
    void enviarArchivo(FileManager& filemanager);
};



#endif //CLIENTE_H

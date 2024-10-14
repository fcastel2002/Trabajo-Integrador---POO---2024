//
// Created by Pachi on 09/10/2024.
//

#ifndef CLIENTE_H
#define CLIENTE_H
#include <string>
#include "FileManager.h"
#include "./XMLRPC++/XmlRpc.h"

class Cliente {
private:
    XmlRpc::XmlRpcClient* cliente;
    std::string m_ipServer{};
    int m_puertoServer{};

public:
    Cliente(std::string ipServer, int puertoServer);
};



#endif //CLIENTE_H

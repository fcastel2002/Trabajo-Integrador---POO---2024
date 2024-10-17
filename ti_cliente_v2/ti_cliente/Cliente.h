#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>
#include "XmlRpc.h"

class Cliente {
public:
    Cliente(const std::string& serverUrl);
    bool conectar(const std::string& usuario, const std::string& clave);
    bool desconectar(const std::string& usuario, const std::string& clave);

private:
    std::string serverUrl;
    XmlRpc::XmlRpcClient client;
};

#endif // CLIENTE_H

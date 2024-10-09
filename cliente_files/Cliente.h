//
// Created by Pachi on 09/10/2024.
//

#ifndef CLIENTE_H
#define CLIENTE_H
#include <string>


class Cliente {
private:
    std::string m_ipAddress{};
    int m_port{};
    bool conected = false;
public:
    Cliente(std::string ipAddress, int puerto) : m_ipAddress(ipAddress), m_port(puerto) {}
    bool autenticar(std::string nombreUsuario, std::string clave);
    bool conectar();
    void desconectar();
    std::string sendCommand(std::string cmd);
    void subirGcode(std::string archivo);
};



#endif //CLIENTE_H

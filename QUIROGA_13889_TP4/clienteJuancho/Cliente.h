#ifndef CLIENTE_H
#define CLIENTE_H

#include <iostream>
#include <string>
#include <vector>
#include "windows.h"
#include "..\librerias\XmlRpc.h"
#include "Interfaz.h"

class ClienteRobot {
private:
    string servidorUrl;
    int servidorPuerto;
    string alias;
    string ip;
    Interfaz interfaz;

public:
    ClienteRobot(const string& url, int puerto);
    void setAlias(const string& nuevoAlias, const string& nuevaIP);
    string crearMensajeJson(const string& comando, const string& usuario, const string& clave, const vector<string>& tipoParametros, const vector<string>& parametros);
    void ejecutarComando(const string& comando, const string& mensajeJson);
    void mostrarMenu();
};

#endif // CLIENTE_H

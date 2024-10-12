#ifndef CLIENTE_H
#define CLIENTE_H

#include "windows.h"
#include "..\librerias\XmlRpc.h"
#include <iostream>
#include <vector>
#include <string>
#include "Interfaz.h"

using namespace std;
using namespace XmlRpc;

class ClienteNumeros {
private:
    string servidorUrl;
    int servidorPuerto;
    vector<double> numerosRecibidos;
    vector<string> tiemposRecibidos;
    string alias;
    string ip;
    double ultimaCotaInferior;
    double ultimaCotaSuperior;
    Interfaz interfaz;  // Agregado para gestionar la E/S

public:
    ClienteNumeros(const string& url, int puerto);
    void setAlias(const string& nuevoAlias, const string& ip);
    void solicitarNumero(bool conTiempo = false);
    void iniciarNuevaSecuencia();
    void mostrarEstadisticas();
    void mostrarNumerosEnOrden();
};

#endif




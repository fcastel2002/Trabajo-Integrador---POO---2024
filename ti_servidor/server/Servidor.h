#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "windows.h"
#include "..\librerias\XmlRpc.h"
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include "Interfaz.h"

using namespace std;
using namespace XmlRpc;

class Usuario {
public:
    string alias;
    string ip;
    int secuenciasIniciadas;
    int numerosGenerados;

    Usuario() : alias(""), ip(""), secuenciasIniciadas(0), numerosGenerados(0) {}

    Usuario(string alias, string ip) : alias(alias), ip(ip), secuenciasIniciadas(0), numerosGenerados(0) {}
};

class ServidorNumeros : public XmlRpcServer {
private:
    map<string, Usuario> usuarios;
    mutex mtx;
    Interfaz interfaz;  // Agregado para gestionar la E/S

public:
    ServidorNumeros();
    void agregarUsuario(const string& alias, const string& ip);
    void registrarSecuencia(const string& alias, const string& ip);
    void agregarNumero(string alias);
    vector<string> obtenerCantidadUsuarios();
    vector<string> obtenerEstadisticasUsuario(string alias);
    void apagarServidor();
    void agregarSecuencia(string alias);
};

class MetodoGenerarNumero : public XmlRpcServerMethod {
private:
    ServidorNumeros* servidor;  // Referencia directa al servidor

public:
    MetodoGenerarNumero(ServidorNumeros* s);  // Constructor que recibe el servidor
    void execute(XmlRpcValue& params, XmlRpcValue& result) override;
};

class MetodoRegistrarSecuencia : public XmlRpcServerMethod {
private:
    ServidorNumeros* servidor;

public:
    MetodoRegistrarSecuencia(ServidorNumeros* s);
    void execute(XmlRpcValue& params, XmlRpcValue& result) override;
};

#endif


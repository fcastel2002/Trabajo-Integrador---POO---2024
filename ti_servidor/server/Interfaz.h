#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Interfaz {
public:
    // Métodos de entrada
    string solicitarAlias();
    int solicitarOpcionMenuServidor();

    void mostrarMensaje(const string& mensaje);
};

#endif

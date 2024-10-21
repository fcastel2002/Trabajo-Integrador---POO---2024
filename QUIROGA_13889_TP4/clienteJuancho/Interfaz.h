#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Interfaz {
public:
    void mostrarMensaje(const string& mensaje);
    void mostrarOpciones();
    string solicitarComando();
    string solicitarUsuario();
    string solicitarClave();
    vector<string> solicitarParametros(const string& comando);
};

#endif // INTERFAZ_H


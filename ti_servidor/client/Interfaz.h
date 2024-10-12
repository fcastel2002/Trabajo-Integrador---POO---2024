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
    double solicitarCotaInferior(double cotaAnterior);
    double solicitarCotaSuperior(double cotaAnterior);
    int solicitarOpcionMenuCliente();
    string solicitarIP();

    // Métodos de salida
    void mostrarResultado(double numero, const string& tiempo);
    void mostrarResultadoSinTiempo(double numero);
    void mostrarEstadisticas(int cantidadNumeros, double suma, double promedio);
    void mostrarNumerosOrdenados(const vector<double>& numeros);
    void mostrarMensaje(const string& mensaje);
};

#endif

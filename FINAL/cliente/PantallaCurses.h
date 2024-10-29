#pragma once
#include <string>
#include <vector>
#include "IPantalla.h"
#include "ErrorHandler.h"

class PantallaCurses:public IPantalla {
public:
    PantallaCurses();
    ~PantallaCurses();
    int mostrarMenu(const std::vector<std::string>& opciones, const std::string& tituloMenu);
    void limpiarPantalla();
    void refrescarPantalla();
    void mostrarTexto(const std::string& mensaje);
    std::string capturarEntrada(const std::string& mensaje, ErrorHandler& errorHandler);
    std::vector<std::string> capturarEntradaMultiple(std::string& mensaje, ErrorHandler& errorHandler);
    void mostrarError(const std::string& error);
    std::string capturarEleccion(const std::string& mensaje, const std::vector<std::string>& opciones, ErrorHandler& errorHandler);
};
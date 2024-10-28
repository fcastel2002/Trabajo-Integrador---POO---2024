#pragma once
#include <string>
#include <vector>

class IPantalla {
public:
    virtual ~IPantalla() = default;

    virtual void mostrarTexto(const std::string& mensaje) = 0;
    virtual void limpiarPantalla() = 0;
    virtual std::string capturarEntrada(const std::string& mensaje) = 0;
    virtual int mostrarMenu(const std::vector<std::string>& opciones) = 0; 
    virtual void refrescarPantalla() = 0;
};

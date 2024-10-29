#pragma once
#include <string>
#include <vector>

class IPantalla {
public:
    virtual ~IPantalla() = default;

    virtual void mostrarTexto(const std::string& mensaje) = 0;
	virtual void mostrarError(const std::string& error) = 0;
    virtual void limpiarPantalla() = 0;

    virtual std::string capturarEntrada(const std::string& mensaje) = 0;
	virtual std::vector<std::string> archivoToVector(std::string& mensaje) = 0;
    virtual std::string capturarEleccion(const std::string& mensaje, const std::vector<std::string>& opciones) = 0;
    virtual int mostrarMenu(const std::vector<std::string>& opciones, const std::string& tituloMenu) = 0; 
    virtual void refrescarPantalla() = 0;
};

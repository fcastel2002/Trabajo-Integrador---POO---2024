#pragma once
#include "IPantalla.h"
#include "Archivo.h"
class PantallaCurses :
    public IPantalla
{
public:
    PantallaCurses();
	~PantallaCurses() override;

	int mostrarMenu(const std::vector<std::string>& opciones, const std::string& tituloMenu) override;
	void mostrarTexto(const std::string& mensaje) override;
	void limpiarPantalla() override;
	std::string capturarEntrada(const std::string& mensaje) override;
	std::vector<std::string> capturarEntradaMultiple(const std::string& mensaje) override;
	std::string capturarEleccion(const std::string& mensaje, const std::vector<std::string>& opciones) override;
	void refrescarPantalla() override;
	void  mostrarError(const std::string& error) override;

};


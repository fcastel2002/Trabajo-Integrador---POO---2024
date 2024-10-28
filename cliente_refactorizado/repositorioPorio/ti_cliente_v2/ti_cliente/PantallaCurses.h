#pragma once
#include "IPantalla.h"
class PantallaCurses :
    public IPantalla
{
public:
    PantallaCurses();
	~PantallaCurses() override;

	int mostrarMenu(const std::vector<std::string>& opciones) override;
	void mostrarTexto(const std::string& mensaje) override;
	void limpiarPantalla() override;
	std::string capturarEntrada(const std::string& mensaje) override;
	void refrescarPantalla() override;
};


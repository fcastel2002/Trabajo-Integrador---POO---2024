#pragma once
#include "Orden.h"
#include <string>

#include <vector>
#include <map>

using namespace std;
class OrdenBuilder {
private:
	string usuario{};
	string clave{};
	string comando{};
	vector<string> parametros{};
	map<string, vector<string>> etiquetasParametros{};
public:
	
	OrdenBuilder() {
		etiquetasParametros["conectar"] = { "Ingrese puerto COM: ", "Ingrese velocidad de transmisión: " };
		etiquetasParametros["mover_efector"] = { "Ingrese X: ", "Ingrese Y: ", "Ingrese Z: ", "Ingrese Velocidad: " };
	}
	OrdenBuilder& conUsuario(const string& usuario) {
		this->usuario = usuario;
		return *this;
	}
	
	OrdenBuilder& conClave(const string& clave) {
		this->clave = clave;
		return *this;
	}

	OrdenBuilder& conComando(const string& comando) {
		this->comando = comando;
		return *this;
	}

	OrdenBuilder& conParametros(const vector<string>& parametros) {
		this->parametros = parametros;
		return *this;
	}

	vector<string> obtenerEtiquetasParametros() const  {
		auto it = etiquetasParametros.find(comando);
		return it != etiquetasParametros.end() ? it->second : vector<string>();
	}

	Orden build() const {
		vector<vector<string>> parametrosOrden = { {usuario}, {clave}, {comando}, parametros };
		return Orden(parametrosOrden);
	}
};
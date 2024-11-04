#pragma once
#include "Orden.h"
#include "ErrorHandler.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class OrdenBuilder {
private:
    string usuario;
    string clave;
    string comando;
    vector<string> parametros;
    map<string, vector<string>> etiquetasParametros;

public:
    OrdenBuilder() {
        etiquetasParametros["Conectarse al Robot"] = { "Ingrese puerto COM: ", "Ingrese velocidad de transmision: " };
        etiquetasParametros["Mover efector"] = { "Ingrese X: ", "Ingrese Y: ", "Ingrese Z: ", "Ingrese Velocidad (opcional): " };
        etiquetasParametros["Aprendizaje"] = {"Iniciar", "Detener"};
        etiquetasParametros["Actuar efector"] = {"Activar", "Desactivar"};
    }

    // Setea el usuario para la orden
    OrdenBuilder& conUsuario(const string& usuario) {
        this->usuario = usuario;
        return *this;
    }

    // Setea la clave para la orden
    OrdenBuilder& conClave(const string& clave) {
        this->clave = clave;
        return *this;
    }

    // Setea el comando de la orden
    OrdenBuilder& conComando(const string& comando) {
        this->comando = comando;
        return *this;
    }

    // Setea los par�metros para la orden
    OrdenBuilder& conParametros(const vector<string>& parametros) {
        this->parametros = parametros;
        return *this;
    }

    // Obtiene las etiquetas de par�metros asociadas al comando
    vector<string> obtenerEtiquetasParametros() const {
        auto it = etiquetasParametros.find(comando);
        return it != etiquetasParametros.end() ? it->second : vector<string>();
    }

    // Construye y retorna una instancia de Orden
    Orden build() const {
        ErrorHandler errorHandler;
        try {
            if (usuario.empty() || clave.empty() || comando.empty()) {
                throw std::runtime_error("Faltan parametros esenciales para construir la orden.");
            }
            std::vector<std::vector<std::string>> parametrosOrden = { {usuario}, {clave}, {comando}, parametros };
            return Orden(parametrosOrden);  // Ahora deber�a funcionar correctamente
        }
        catch (const std::exception& e) {
            errorHandler.handleException(e);
            return Orden(std::vector<std::vector<std::string>>{});  // Retorna una orden vac�a si falla
        }
    }
};

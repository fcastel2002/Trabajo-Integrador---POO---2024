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
    /**
     * Constructor de la clase OrdenBuilder.
     * Inicializa las etiquetas de parámetros para los diferentes comandos.
     */
    OrdenBuilder() {
        etiquetasParametros["Conectarse al Robot"] = { "Ingrese puerto COM: ", "Ingrese velocidad de transmision: " };
        etiquetasParametros["Mover efector"] = { "Ingrese X: ", "Ingrese Y: ", "Ingrese Z: ", "Ingrese Velocidad (opcional): " };
        etiquetasParametros["Aprendizaje"] = { "Iniciar", "Detener" };
        etiquetasParametros["Actuar efector"] = { "Activar", "Desactivar" };
    }

    /**
     * Setea el usuario para la orden.
     * @param usuario Nombre del usuario.
     * @return Referencia a la instancia actual de OrdenBuilder.
     */
    OrdenBuilder& conUsuario(const string& usuario) {
        this->usuario = usuario;
        return *this;
    }

    /**
     * Setea la clave para la orden.
     * @param clave Clave del usuario.
     * @return Referencia a la instancia actual de OrdenBuilder.
     */
    OrdenBuilder& conClave(const string& clave) {
        this->clave = clave;
        return *this;
    }

    /**
     * Setea el comando de la orden.
     * @param comando Comando a ejecutar.
     * @return Referencia a la instancia actual de OrdenBuilder.
     */
    OrdenBuilder& conComando(const string& comando) {
        this->comando = comando;
        return *this;
    }

    /**
     * Setea los parámetros para la orden.
     * @param parametros Vector de strings con los parámetros.
     * @return Referencia a la instancia actual de OrdenBuilder.
     */
    OrdenBuilder& conParametros(const vector<string>& parametros) {
        this->parametros = parametros;
        return *this;
    }

    /**
     * Obtiene las etiquetas de parámetros asociadas al comando.
     * @return Vector de strings con las etiquetas de parámetros.
     */
    vector<string> obtenerEtiquetasParametros() const {
        auto it = etiquetasParametros.find(comando);
        return it != etiquetasParametros.end() ? it->second : vector<string>();
    }

    /**
     * Construye y retorna una instancia de Orden.
     * @return Instancia de Orden.
     */
    Orden build() const {
        ErrorHandler errorHandler;
        try {
            if (usuario.empty() || clave.empty() || comando.empty()) {
                throw std::runtime_error("Faltan parametros esenciales para construir la orden.");
            }
            std::vector<std::vector<std::string>> parametrosOrden = { {usuario}, {clave}, {comando}, parametros };
            return Orden(parametrosOrden);  
        }
        catch (const std::exception& e) {
            errorHandler.handleException(e);
            return Orden(std::vector<std::vector<std::string>>{});  // Retorna una orden vacía si falla
        }
    }
};

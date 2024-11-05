#pragma once
#include <string>
#include <vector>
#include <map>
#include "XmlRpc.h"
#include "ErrorHandler.h"

using namespace XmlRpc;

class Orden {
private:
    std::string m_tipo;
    std::vector<std::vector<std::string>> m_parametros;
    std::map<std::string, std::vector<std::string>> etiquetasParametros;

public:
    /**
     * Constructor que acepta un vector de vectores de strings.
     * @param parametros Vector de vectores de strings que representan los parámetros de la orden.
     */
    explicit Orden(const std::vector<std::vector<std::string>>& parametros) : m_parametros{ parametros } {}

    /**
     * Crea una orden en formato XmlRpcValue a partir de los parámetros proporcionados.
     * @return XmlRpcValue que representa la orden creada.
     */
    XmlRpcValue crearOrden() {
        XmlRpcValue params;
        ErrorHandler errorHandler;

        try {
            if (m_parametros.size() < 4) {
                throw std::runtime_error("Parametros insuficientes para crear la orden.");
            }

            params[0] = m_parametros[0][0];
            params[1] = m_parametros[1][0];
            params[2] = m_parametros[2][0];

            for (int i = 0; i < m_parametros[3].size(); i++) {
                params[3][i] = m_parametros[3][i];
            }
        }
        catch (const std::exception& e) {
            errorHandler.handleException(e);
        }

        return params;
    }
};

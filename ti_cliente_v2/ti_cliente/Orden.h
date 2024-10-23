#pragma once
#include <string>
#include <vector>
#include "XmlRpc.h"

using namespace  XmlRpc;
class Orden {
private:
    std::string m_tipo{};
    std::vector<std::string> m_parametros{};
    // Atributo normal para almacenar las etiquetas de los parámetros
    std::map<std::string, std::vector<std::string>> etiquetasParametros;




public:

    Orden(std::vector<std::string>& parametros) :m_parametros{ parametros } {}

    XmlRpcValue crearOrden(std::string& user, std::string& pass) {

    XmlRpcValue params;
    params[0] = m_parametros[0]; //user
    params[1] = m_parametros[1]; //pass
    params[2] = m_parametros[2];
    for (int i = 0; i < m_parametros[3].size(); i++) {
        params[3][i] = m_parametros[3][i];
    }
    return params;
    }
};
#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
class Orden {
private:
    std::string m_tipo{};
    std::vector<std::string> m_parametros{};
    // Atributo normal para almacenar las etiquetas de los parámetros
    std::map<std::string, std::vector<std::string>> etiquetasParametros;

public:
    Orden(const std::string& tipo, const std::vector<std::string>& parametros)
        : m_tipo(tipo), m_parametros(parametros) {
        // Inicializamos el mapa de etiquetas en el constructor
        etiquetasParametros = {
            {"mover_efector", {"X", "Y", "Z", "velocidad"}},
            {"conectar", {"puerto_COM", "tasa_baudios"}},
            {"actuar_efector", {"accion_efector"}},
            {"ejecutar_automatico", {"nombre_archivo"}}
        };
    }

    std::string toJson(std::string& username, std::string& pass) const {
        nlohmann::json j;
        j["usuario"] = username;
        j["clave"] = pass;
        j["comando"] = m_tipo;

        // Seleccionamos las etiquetas de los parámetros dependiendo del tipo de comando
        nlohmann::json parametrosJson;
        auto it = etiquetasParametros.find(m_tipo);
        if (it != etiquetasParametros.end()) {
            const std::vector<std::string>& etiquetas = it->second;
            for (size_t i = 0; i < etiquetas.size() && i < m_parametros.size(); ++i) {
                parametrosJson[etiquetas[i]] = m_parametros[i];
            }
        }

        j["parametros"] = parametrosJson;
        return j.dump();
    }
};
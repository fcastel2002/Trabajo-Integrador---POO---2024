#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
class Orden {
private:
	std::string m_tipo{};
	std::vector<std::string> m_parametros{};
public:

	Orden(const std::string& tipo, const std::vector<std::string>& parametros) : m_tipo(tipo), m_parametros(parametros) {}

	std::string toJson(std::string& username, std::string& pass) const {
		nlohmann::json j;
		j["usuario"] = username;
		j["clave"] = pass;
		j["comando"] = m_tipo;
		j["parametros"] = m_parametros;
		j["cantidad_param"] = m_parametros.size();
		return j.dump();
	}	

};
#pragma once
#include<string>



class Respuesta {
private:
	std::string m_usuario;
	std::string m_comando;
	std::string m_contenido;	
	
public:
	
	Respuesta(const std::string& usuario, const std::string& comando, const std::string& contenido)
		: m_usuario{ usuario }
		, m_comando{ comando }
		, m_contenido{ contenido } {}	

	std::string getUsuario() const {
		return m_usuario;
	}

	std::string getComando() const {
		return m_comando;
	}

	std::string getContenido() const {
		return m_contenido;
	}
	

};
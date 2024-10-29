#pragma once

#include "Respuesta.h"

#include <string>

class RespuestaBuilder {
private:
	std::string m_usuario;
	std::string m_comando;
	std::string m_contenido;
public:

	RespuestaBuilder& conUsuario(const std::string& usuario) {
		m_usuario = usuario;
		return *this;
	}

	RespuestaBuilder& conComando(const std::string& comando) {
		m_comando = comando;
		return *this;
	}

	RespuestaBuilder& conContenido(const std::string& contenido) {
		m_contenido = contenido;
		return *this;
	}

	Respuesta build() {
		return Respuesta(m_usuario, m_comando, m_contenido);
	}
};


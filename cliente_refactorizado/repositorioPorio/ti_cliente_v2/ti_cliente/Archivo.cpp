#include "Archivo.h"


Archivo::Archivo(std::string nombre, std::string ruta)
	: m_nombre{ nombre }
	, m_ruta{ ruta }
	, m_archivo{ m_ruta + m_nombre, std::ios::in }
	, m_contenido{} {}

Archivo::~Archivo() {
	if (m_archivo.is_open()){
		cerrar();
	}
}


bool Archivo::abrir() {
	if (m_archivo.is_open()) {
		return true;
	}
	m_archivo.open(m_ruta + m_nombre, std::ios::in);
	if (m_archivo.is_open()) {
		return true;
	}
	return false;
}

bool Archivo::cerrar() {
	if (m_archivo.is_open()) {
		m_archivo.close();
		return true;
	}
	return false;
}

bool Archivo::leer() {
	if (!m_archivo.is_open()) {
		if (!abrir()) {
			return false;

		}
	}

	std::string linea;
	while (std::getline(m_archivo, linea)) {
		m_contenido.push_back(linea);
	}

	cerrar();
	return true;
}

std::vector<std::string> Archivo::getContenido() {
	return m_contenido;
}

std::string Archivo::getNombre() {
	return m_nombre;
}

std::string Archivo::getRuta() {
	return m_ruta;
}

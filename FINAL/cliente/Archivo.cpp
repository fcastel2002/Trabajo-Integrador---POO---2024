#include "Archivo.h"
#include "ErrorHandler.h"

Archivo::Archivo(std::string nombre, std::string ruta)
    : m_nombre{ nombre + ".txt" }
    , m_ruta{ ruta }
    , m_archivo{}
    , m_contenido{} {}

Archivo::~Archivo() {
    if (m_archivo.is_open()) {
        cerrar();
    }
}

bool Archivo::abrir() {
    if (m_archivo.is_open()) {
        return true;
    }

    m_archivo.open(m_ruta + m_nombre, std::ios::in);
    if (!m_archivo.is_open()) {
        ErrorHandler errorHandler;
        errorHandler.logError(ErrorCode::FILE_NOT_FOUND, ErrorLevel::ERROR);
        errorHandler.displayError("No se pudo abrir el archivo: " + m_nombre, ErrorLevel::ERROR);
        return false;
    }

    return true;
}

bool Archivo::cerrar() {
    if (m_archivo.is_open()) {
        m_archivo.close();
        if (m_archivo.fail()) {  // Verificamos si hubo error al cerrar
            ErrorHandler errorHandler;
            errorHandler.logError("Error al cerrar el archivo: " + m_nombre, ErrorLevel::WARNING);
            errorHandler.displayError("Error al cerrar el archivo: " + m_nombre, ErrorLevel::WARNING);
            return false;
        }
        return true;
    }
    return false;
}

bool Archivo::leer() {
    if (!abrir()) {  // Asegura que el archivo esté abierto o intenta abrirlo
        return false;
    }

    std::string linea;
    while (std::getline(m_archivo, linea)) {
        m_contenido.push_back(linea);
    }

    if (m_archivo.bad()) {  // Verifica si hubo error al leer
        ErrorHandler errorHandler;
        errorHandler.logError(ErrorCode::OPERATION_FAILED, ErrorLevel::ERROR);
        errorHandler.displayError("Error al leer el archivo: " + m_nombre, ErrorLevel::ERROR);
        cerrar();
        return false;
    }

    //cerrar();
    return true;
}


std::vector<std::string> Archivo::archivoToVector(Archivo& archivo_) {
	ErrorHandler errorHandler;

	try {
		//Archivo archivo_gcode(nombreArchivo, "");
		if (!archivo_.abrir()) {
			errorHandler.logError(ErrorCode::FILE_NOT_FOUND, ErrorLevel::ERROR);
			errorHandler.displayError("No se pudo abrir el archivo: " + archivo_.m_nombre, ErrorLevel::ERROR);
			return {};
		}

		if (!archivo_.leer()) {
			errorHandler.logError(ErrorCode::OPERATION_FAILED, ErrorLevel::ERROR);
			errorHandler.displayError("Error al leer el archivo: " + archivo_.m_nombre, ErrorLevel::ERROR);
			return {};
		}

		return archivo_.getContenido();

	}
	catch (const std::exception& e) {
		errorHandler.handleException(e);
		errorHandler.displayError("Excepcion al procesar el archivo.", ErrorLevel::ERROR);
		return {};
	}
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

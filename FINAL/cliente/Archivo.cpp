#include "Archivo.h"
#include "ErrorHandler.h"

Archivo::Archivo(std::string nombre, std::string ruta)
    : m_nombre{ nombre }, m_ruta{ ruta }, m_archivo{ m_ruta + m_nombre, std::ios::in }, m_contenido{} {}

Archivo::~Archivo() {
    if (m_archivo.is_open()) {
        ErrorHandler errorHandler;  // Crear una instancia temporal si es necesario manejar errores en el cierre
        cerrar(errorHandler);
    }
}

bool Archivo::abrir(ErrorHandler& errorHandler) {
    if (m_archivo.is_open()) {
        return true;
    }
    m_archivo.open(m_ruta + m_nombre, std::ios::in);
    if (!m_archivo.is_open()) {
        errorHandler.logError(ErrorCode::FILE_NOT_FOUND, ErrorLevel::ERROR);
        return false;
    }
    return true;
}

bool Archivo::cerrar(ErrorHandler& errorHandler) {
    if (m_archivo.is_open()) {
        m_archivo.close();
        if (m_archivo.fail()) {
            errorHandler.logError("Error al cerrar el archivo.", ErrorLevel::ERROR);
            return false;
        }
        return true;
    }
    return false;
}

bool Archivo::leer(ErrorHandler& errorHandler) {
    if (!m_archivo.is_open() && !abrir(errorHandler)) {
        errorHandler.logError(ErrorCode::FILE_NOT_FOUND, ErrorLevel::ERROR);
        return false;
    }

    std::string linea;
    while (std::getline(m_archivo, linea)) {
        if (m_archivo.fail()) {
            errorHandler.logError("Error al leer una línea del archivo.", ErrorLevel::ERROR);
            cerrar(errorHandler);
            return false;
        }
        m_contenido.push_back(linea);
    }

    cerrar(errorHandler);
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

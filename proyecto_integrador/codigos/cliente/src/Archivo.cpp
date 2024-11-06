#include "Archivo.h"
#include "ErrorHandler.h"

/**
 * Constructor que inicializa el nombre y la ruta del archivo.
 * @param nombre Nombre del archivo con extensión.
 * @param ruta Ruta del archivo (opcional).
 */
Archivo::Archivo(std::string nombre, std::string ruta)
    : m_nombre{ nombre }
    , m_ruta{ ruta }
    , m_archivo{}
    , m_contenido{} {}

/**
 * Destructor que cierra el archivo si está abierto.
 */
Archivo::~Archivo() {
    if (m_archivo.is_open()) {
        cerrar();
    }
}

/**
 * Abre el archivo para lectura.
 * @return true si el archivo se abrió correctamente, false en caso contrario.
 */
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

/**
 * Cierra el archivo si está abierto.
 * @return true si el archivo se cerró correctamente, false en caso contrario.
 */
bool Archivo::cerrar() {
    if (m_archivo.is_open()) {
        m_archivo.close();
        }
    return false;
}

/**
 * Lee el contenido del archivo línea por línea.
 * @return true si la lectura fue exitosa, false en caso contrario.
 */
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
        errorHandler.displayError("Error al leer el archivo: " + m_ruta+ m_nombre, ErrorLevel::ERROR);
        cerrar();
        return false;
    }

    return true;
}

/**
 * Convierte el contenido del archivo en un vector de strings.
 * @param archivo_ Referencia a un objeto de la clase Archivo.
 * @return Vector de strings con el contenido del archivo.
 */
std::vector<std::string> Archivo::archivoToVector(Archivo& archivo_) {
    ErrorHandler errorHandler;

    try {
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

/**
 * Obtiene una lista de archivos en la ruta especificada.
 * @param ruta Ruta del directorio.
 * @return Vector de strings con los nombres de los archivos en la ruta.
 */
std::vector<std::string> Archivo::obtenerArchivos(const std::string& ruta) {
    std::vector<std::string> archivos;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(ruta)) {
            archivos.push_back(entry.path().filename().string());
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        ErrorHandler errorHandler;
        errorHandler.logError(ErrorCode::FILE_NOT_FOUND, ErrorLevel::ERROR);
        errorHandler.displayError("No se pudo acceder al directorio: " + ruta, ErrorLevel::ERROR);
    }
    return archivos;
}

/**
 * Retorna el contenido del archivo leído en un vector de strings.
 * @return Vector de strings con el contenido del archivo.
 */
std::vector<std::string> Archivo::getContenido() {
    return m_contenido;
}

/**
 * Obtiene el nombre del archivo.
 * @return Nombre del archivo.
 */
std::string Archivo::getNombre() {
    return m_nombre;
}

/**
 * Obtiene la ruta del archivo.
 * @return Ruta del archivo.
 */
std::string Archivo::getRuta() {
    return m_ruta;
}

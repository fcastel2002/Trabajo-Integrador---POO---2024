#include "ErrorHandler.h"
#include <ctime>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include "PantallaCurses.h"

/**
 * Constructor: abre el archivo de log.
 * @param logFilePath Ruta del archivo de log.
 */
ErrorHandler::ErrorHandler(const std::string& logFilePath) : logFilePath(logFilePath) {
    openLogFile();
    m_pantalla = new PantallaCurses();
}

/**
 * Abre el archivo de log para escritura.
 */
void ErrorHandler::openLogFile() {
    logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        m_pantalla->mostrarError("No se pudo abrir el archivo de log de errores.");
        logError("No se pudo abrir el archivo de log de errores.", ErrorLevel::ERROR);
    }
}

/**
 * Cierra el archivo de log si está abierto.
 */
void ErrorHandler::closeLogFile() {
    if (logFile.is_open()) {
        logFile.close();
        if (logFile.fail()) {
            m_pantalla->mostrarError("Error al cerrar el archivo de log.");
        }
    }
}

/**
 * Convierte el nivel de error en una cadena legible.
 * @param level Nivel de error.
 * @return Cadena representando el nivel de error.
 */
std::string ErrorHandler::getLevelString(ErrorLevel level) const {
    static const std::unordered_map<ErrorLevel, std::string> levelStrings = {
        {ErrorLevel::INFO, "INFO"},
        {ErrorLevel::WARNING, "WARNING"},
        {ErrorLevel::ERROR, "ERROR"}
    };
    auto it = levelStrings.find(level);
    return it != levelStrings.end() ? it->second : "UNKNOWN";
}

/**
 * Obtiene el mensaje de error correspondiente a un código de error.
 * @param code Código de error.
 * @return Mensaje de error.
 */
std::string ErrorHandler::getErrorMessage(ErrorCode code) const {
    static const std::unordered_map<ErrorCode, std::string> errorMessages = {
        {ErrorCode::FILE_NOT_FOUND, "Archivo no encontrado"},
        {ErrorCode::CONNECTION_FAILED, "Fallo la conexion"},
        {ErrorCode::INVALID_INPUT, "Entrada invalida"},
        {ErrorCode::OPERATION_FAILED, "La operacion fallo"},
        {ErrorCode::UNKNOWN_ERROR, "Error desconocido"}
    };
    auto it = errorMessages.find(code);
    return it != errorMessages.end() ? it->second : "Error desconocido";
}

/**
 * Agrega timestamp al mensaje de error y lo registra en el archivo de log.
 * @param message Mensaje de error.
 * @param level Nivel de error.
 */
void ErrorHandler::logError(const std::string& message, ErrorLevel level) {
    std::string levelStr = getLevelString(level);
    std::string timestamp = getCurrentTimestamp();
    std::string logMessage = timestamp + " " + levelStr + ": " + message;

    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
    }
    else {
        m_pantalla->mostrarError("Error al escribir en el log: " + logMessage);
    }
}

/**
 * Registra el error utilizando un código predefinido.
 * @param code Código de error.
 * @param level Nivel de error.
 */
void ErrorHandler::logError(ErrorCode code, ErrorLevel level) {
    std::string message = getErrorMessage(code);
    logError(message, level);
}

/**
 * Muestra el error al usuario.
 * @param message Mensaje de error.
 * @param level Nivel de error.
 */
void ErrorHandler::displayError(const std::string& message, ErrorLevel level) {
    std::string levelStr = getLevelString(level);
    m_pantalla->mostrarError("[" + levelStr + "] " + message);
}

/**
 * Maneja una excepción y registra el error.
 * @param e Excepción capturada.
 */
void ErrorHandler::handleException(const std::exception& e) {
    logError(e.what(), ErrorLevel::ERROR);
    displayError("Excepcion capturada: " + std::string(e.what()), ErrorLevel::ERROR);
}

/**
 * Genera el timestamp actual.
 * @return Timestamp actual en formato de cadena.
 */
std::string ErrorHandler::getCurrentTimestamp() const {
    auto now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "[%Y-%m-%d %H:%M:%S]");
    return oss.str();
}

/**
 * Destructor: cierra el archivo de log si está abierto.
 */
ErrorHandler::~ErrorHandler() {
    closeLogFile();
}

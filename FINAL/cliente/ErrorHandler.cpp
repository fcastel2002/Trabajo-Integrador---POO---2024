// ErrorHandler.cpp
#include "ErrorHandler.h"
#include <ctime>
#include <unordered_map>
#include <iomanip>
#include <sstream>

// Constructor: abre el archivo de log
ErrorHandler::ErrorHandler(const std::string& logFilePath) : logFilePath(logFilePath) {
    openLogFile();
}

void ErrorHandler::openLogFile() {
    logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de log de errores." << std::endl;
        logError("No se pudo abrir el archivo de log de errores.", ErrorLevel::ERROR);
    }
}

void ErrorHandler::closeLogFile() {
    if (logFile.is_open()) {
        logFile.close();
        if (logFile.fail()) {
            std::cerr << "Error al cerrar el archivo de log." << std::endl;
        }
    }
}

// Convierte el nivel de error en una cadena legible
std::string ErrorHandler::getLevelString(ErrorLevel level) const {
    static const std::unordered_map<ErrorLevel, std::string> levelStrings = {
        {ErrorLevel::INFO, "INFO"},
        {ErrorLevel::WARNING, "WARNING"},
        {ErrorLevel::ERROR, "ERROR"}
    };
    auto it = levelStrings.find(level);
    return it != levelStrings.end() ? it->second : "UNKNOWN";
}

// Convierte el código de error en un mensaje específico
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

// Agrega timestamp al mensaje de error
void ErrorHandler::logError(const std::string& message, ErrorLevel level) {
    std::string levelStr = getLevelString(level);
    std::string timestamp = getCurrentTimestamp();
    std::string logMessage = timestamp + " " + levelStr + ": " + message;

    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
    }
    else {
        std::cerr << "Error al escribir en el log: " << logMessage << std::endl;
    }
}

// Registra el error utilizando un código predefinido
void ErrorHandler::logError(ErrorCode code, ErrorLevel level) {
    std::string message = getErrorMessage(code);
    logError(message, level);
}

// Muestra el error al usuario
void ErrorHandler::displayError(const std::string& message, ErrorLevel level) {
    std::string levelStr = getLevelString(level);
    std::cerr << "[" << levelStr << "] " << message << std::endl;
}

// Maneja una excepción y registra el error
void ErrorHandler::handleException(const std::exception& e) {
    logError(e.what(), ErrorLevel::ERROR);
    displayError("Excepcion capturada: " + std::string(e.what()), ErrorLevel::ERROR);
}

// Genera el timestamp actual
std::string ErrorHandler::getCurrentTimestamp() const {
    auto now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "[%Y-%m-%d %H:%M:%S]");
    return oss.str();
}

ErrorHandler::~ErrorHandler() {
    closeLogFile();
}

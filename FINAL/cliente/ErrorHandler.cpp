// ErrorHandler.cpp
#include "ErrorHandler.h"

// Constructor: abre el archivo de log
ErrorHandler::ErrorHandler(const std::string& logFilePath) : logFilePath(logFilePath) {
    openLogFile();
}

void ErrorHandler::openLogFile() {
    logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de log de errores." << std::endl;
    }
}

void ErrorHandler::closeLogFile() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Convierte el nivel de error en una cadena legible
std::string ErrorHandler::getLevelString(ErrorLevel level) const {
    switch (level) {
        case ErrorLevel::INFO: return "INFO";
        case ErrorLevel::WARNING: return "WARNING";
        case ErrorLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// Convierte el código de error en un mensaje específico
std::string ErrorHandler::getErrorMessage(ErrorCode code) const {
    switch (code) {
        case ErrorCode::FILE_NOT_FOUND: return "Archivo no encontrado";
        case ErrorCode::CONNECTION_FAILED: return "Falló la conexión";
        case ErrorCode::INVALID_INPUT: return "Entrada inválida";
        case ErrorCode::OPERATION_FAILED: return "La operación falló";
        default: return "Error desconocido";
    }
}

// Registra el mensaje de error con nivel
void ErrorHandler::logError(const std::string& message, ErrorLevel level) {
    std::string levelStr = getLevelString(level);
    std::string logMessage = levelStr + ": " + message;

    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
    } else {
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
    displayError("Excepción capturada: " + std::string(e.what()), ErrorLevel::ERROR);
}

ErrorHandler::~ErrorHandler() {
    closeLogFile();
}

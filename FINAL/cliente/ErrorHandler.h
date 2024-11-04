// ErrorHandler.h
#pragma once


#include "IPantalla.h"
#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <unordered_map>

// Niveles de severidad de error
enum class ErrorLevel {
    INFO,
    WARNING,
    ERROR
};

// Códigos de errores predefinidos
enum class ErrorCode {
    FILE_NOT_FOUND,
    CONNECTION_FAILED,
    INVALID_INPUT,
    OPERATION_FAILED,
    UNKNOWN_ERROR
};

class ErrorHandler {
private:
    IPantalla* m_pantalla;
    std::string logFilePath;
    std::ofstream logFile;

    // Obtiene la representación en cadena de un nivel de error
    std::string getLevelString(ErrorLevel level) const;

    // Obtiene el mensaje de error para un código de error específico
    std::string getErrorMessage(ErrorCode code) const;

    // Abre el archivo de log para escribir
    void openLogFile();

    // Cierra el archivo de log
    void closeLogFile();

    // Obtiene el timestamp actual para el log
    std::string getCurrentTimestamp() const;
public:
    // Constructor: abre el archivo de log
    ErrorHandler(const std::string& logFilePath = "error_log.txt");

    // Destructor: cierra el archivo de log si está abierto 
    ~ErrorHandler(); 

    // Registra un mensaje de error con un nivel de severidad
    void logError(const std::string& message, ErrorLevel level = ErrorLevel::ERROR);

    // Registra un error usando un código predefinido
    void logError(ErrorCode code, ErrorLevel level = ErrorLevel::ERROR);

    // Muestra el error en la consola con un nivel de severidad
    void displayError(const std::string& message, ErrorLevel level = ErrorLevel::ERROR);

    // Maneja una excepción y registra el error
    void handleException(const std::exception& e);


};

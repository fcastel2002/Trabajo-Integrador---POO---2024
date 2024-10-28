// ErrorHandler.h
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <exception>

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
public:
    ErrorHandler(const std::string& logFilePath = "error_log.txt");
	~ErrorHandler();
    void logError(const std::string& message, ErrorLevel level = ErrorLevel::ERROR);
    void logError(ErrorCode code, ErrorLevel level = ErrorLevel::ERROR);
    void displayError(const std::string& message, ErrorLevel level = ErrorLevel::ERROR);
    void handleException(const std::exception& e);
    
private:
    std::string logFilePath;
    std::ofstream logFile;

    std::string getLevelString(ErrorLevel level) const;
    std::string getErrorMessage(ErrorCode code) const;
    void openLogFile();
    void closeLogFile();
};

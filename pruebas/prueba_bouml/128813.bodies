class ErrorHandler
!!!130477.cpp!!!	ErrorHandler(in logFilePath : std::string = "error_log.txt")

    openLogFile();
!!!130605.cpp!!!	~ErrorHandler()

    closeLogFile();
!!!130733.cpp!!!	logError(in message : std::string, in level : ErrorLevel = ErrorLevel::ERROR) : void

    std::string levelStr = getLevelString(level);
    std::string timestamp = getCurrentTimestamp();
    std::string logMessage = timestamp + " " + levelStr + ": " + message;

    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
    }
    else {
        std::cerr << "Error al escribir en el log: " << logMessage << std::endl;
    }
!!!130861.cpp!!!	logError(in code : ErrorCode, in level : ErrorLevel = ErrorLevel::ERROR) : void

    std::string message = getErrorMessage(code);
    logError(message, level);
!!!130989.cpp!!!	displayError(in message : std::string, in level : ErrorLevel = ErrorLevel::ERROR) : void

    std::string levelStr = getLevelString(level);
    std::cerr << "[" << levelStr << "] " << message << std::endl;
!!!131117.cpp!!!	handleException(in e : std::exception) : void

    logError(e.what(), ErrorLevel::ERROR);
    displayError("Excepcion capturada: " + std::string(e.what()), ErrorLevel::ERROR);
!!!131245.cpp!!!	getLevelString(in level : ErrorLevel) : std::string

    static const std::unordered_map<ErrorLevel, std::string> levelStrings = {
        {ErrorLevel::INFO, "INFO"},
        {ErrorLevel::WARNING, "WARNING"},
        {ErrorLevel::ERROR, "ERROR"}
    };
    auto it = levelStrings.find(level);
    return it != levelStrings.end() ? it->second : "UNKNOWN";
!!!131373.cpp!!!	getErrorMessage(in code : ErrorCode) : std::string

    static const std::unordered_map<ErrorCode, std::string> errorMessages = {
        {ErrorCode::FILE_NOT_FOUND, "Archivo no encontrado"},
        {ErrorCode::CONNECTION_FAILED, "Fallo la conexion"},
        {ErrorCode::INVALID_INPUT, "Entrada invalida"},
        {ErrorCode::OPERATION_FAILED, "La operacion fallo"},
        {ErrorCode::UNKNOWN_ERROR, "Error desconocido"}
    };
    auto it = errorMessages.find(code);
    return it != errorMessages.end() ? it->second : "Error desconocido";
!!!131501.cpp!!!	openLogFile() : void

    logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo de log de errores." << std::endl;
        logError("No se pudo abrir el archivo de log de errores.", ErrorLevel::ERROR);
    }
!!!131629.cpp!!!	closeLogFile() : void

    if (logFile.is_open()) {
        logFile.close();
        if (logFile.fail()) {
            std::cerr << "Error al cerrar el archivo de log." << std::endl;
        }
    }
!!!131757.cpp!!!	getCurrentTimestamp() : std::string

    auto now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "[%Y-%m-%d %H:%M:%S]");
    return oss.str();

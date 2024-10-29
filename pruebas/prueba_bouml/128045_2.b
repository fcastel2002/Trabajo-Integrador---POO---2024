class Archivo
!!!128173.cpp!!!	~Archivo()

    if (m_archivo.is_open()) {
        cerrar();
    }
!!!128301.cpp!!!	abrir() : bool

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
!!!128429.cpp!!!	cerrar() : bool

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
!!!128557.cpp!!!	leer() : bool

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

    cerrar();
    return true;
!!!128685.cpp!!!	getContenido() : std::vector<std::string>

    return m_contenido;
!!!128813.cpp!!!	getNombre() : std::string

    return m_nombre;
!!!128941.cpp!!!	getRuta() : std::string

    return m_ruta;

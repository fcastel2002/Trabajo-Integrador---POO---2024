#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "ErrorHandler.h"

class Archivo {
private:
    std::string m_nombre;
    std::string m_ruta;
    std::fstream m_archivo;
    std::vector<std::string> m_contenido;

public:
    Archivo(std::string nombre, std::string ruta);
    ~Archivo();
    bool abrir(ErrorHandler& errorHandler); 
    bool cerrar(ErrorHandler& errorHandler); 
    bool leer(ErrorHandler& errorHandler); 
    std::vector<std::string> getContenido();
    std::string getNombre();
    std::string getRuta();
};
#pragma once
#include <string>
#include <vector>
#include <fstream>

class Archivo {
private:
    std::string m_nombre;                  // Nombre del archivo con extensión
    std::string m_ruta;                    // Ruta del archivo
    std::fstream m_archivo;                // Flujo de archivo
    std::vector<std::string> m_contenido;  // Contenido leído del archivo

public:
    // Constructor que inicializa el nombre y la ruta del archivo
    Archivo(std::string nombre, std::string ruta = "");

    // Destructor que cierra el archivo si está abierto
    ~Archivo();

    // Abre el archivo para lectura
    bool abrir();

    // Cierra el archivo si está abierto
    bool cerrar();

    // Lee el contenido del archivo línea por línea
    bool leer();

    // Retorna el contenido del archivo leído en un vector de strings
    std::vector<std::string> getContenido();

    // Obtiene el nombre del archivo
    std::string getNombre();

    // Obtiene la ruta del archivo
    std::string getRuta();
};
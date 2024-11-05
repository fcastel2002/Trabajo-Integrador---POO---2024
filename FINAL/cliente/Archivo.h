#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

class Archivo {
private:
    std::string m_nombre;                  // Nombre del archivo con extensión
    std::string m_ruta;                    // Ruta del archivo
    std::fstream m_archivo;                // Flujo de archivo
    std::vector<std::string> m_contenido;  // Contenido leído del archivo

public:
    /**
     * Constructor que inicializa el nombre y la ruta del archivo.
     * @param nombre Nombre del archivo con extensión.
     * @param ruta Ruta del archivo (opcional).
     */
    Archivo(std::string nombre, std::string ruta = "");

    /**
     * Destructor que cierra el archivo si está abierto.
     */
    ~Archivo();

    /**
     * Convierte el contenido del archivo en un vector de strings.
     * @param archivo_ Referencia a un objeto de la clase Archivo.
     * @return Vector de strings con el contenido del archivo.
     */
    std::vector<std::string> archivoToVector(Archivo& archivo_);

    /**
     * Abre el archivo para lectura.
     * @return true si el archivo se abrió correctamente, false en caso contrario.
     */
    bool abrir();

    /**
     * Cierra el archivo si está abierto.
     * @return true si el archivo se cerró correctamente, false en caso contrario.
     */
    bool cerrar();

    /**
     * Lee el contenido del archivo línea por línea.
     * @return true si la lectura fue exitosa, false en caso contrario.
     */
    bool leer();

    /**
     * Retorna el contenido del archivo leído en un vector de strings.
     * @return Vector de strings con el contenido del archivo.
     */
    std::vector<std::string> getContenido();

    /**
     * Obtiene el nombre del archivo.
     * @return Nombre del archivo.
     */
    std::string getNombre();

    /**
     * Obtiene la ruta del archivo.
     * @return Ruta del archivo.
     */
    std::string getRuta();

    /**
     * Obtiene una lista de archivos en la ruta especificada.
     * @param ruta Ruta del directorio.
     * @return Vector de strings con los nombres de los archivos en la ruta.
     */
    static std::vector<std::string> obtenerArchivos(const std::string& ruta);
};

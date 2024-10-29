#pragma once
#include "IPantalla.h"
#include "Archivo.h"
#include "ErrorHandler.h"  // Agregado para manejar errores si es necesario en el .h
#include <vector>
#include <string>

class PantallaCurses : public IPantalla {
public:
    // Constructor que inicializa la pantalla usando PDCurses
    PantallaCurses();

    // Destructor que cierra la pantalla al finalizar
    ~PantallaCurses() override;

    // Muestra el menú y permite seleccionar una opción
    int mostrarMenu(const std::vector<std::string>& opciones, const std::string& tituloMenu) override;

    // Muestra un mensaje de texto en la pantalla
    void mostrarTexto(const std::string& mensaje) override;

    // Limpia el contenido de la pantalla
    void limpiarPantalla() override;

    // Captura la entrada de texto del usuario
    std::string capturarEntrada(const std::string& mensaje) override;

    // Convierte el contenido de un archivo en un vector de strings
    std::vector<std::string> archivoToVector(std::string& mensaje) override;

    // Muestra opciones y captura la elección del usuario
    std::string capturarEleccion(const std::string& mensaje, const std::vector<std::string>& opciones) override;

    // Refresca la pantalla para actualizar su contenido
    void refrescarPantalla() override;

    // Muestra un mensaje de error resaltado en la pantalla
    void mostrarError(const std::string& error) override;
};

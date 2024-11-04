#pragma once
#include "IPantalla.h"
#include "Archivo.h"
#include "ErrorHandler.h"  // Agregado para manejar errores si es necesario en el .h
#include <vector>
#include <curses.h>
#include <string>

class PantallaCurses : public IPantalla {
private:
    WINDOW* userWin;
    std::string user_;
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
	std::string capturarEntrada(const std::string& mensaje, const std::vector<std::string>& opciones) override;
   
    // Refresca la pantalla para actualizar su contenido
    void refrescarPantalla() override;

    // Muestra un mensaje de error resaltado en la pantalla
    void mostrarError(const std::string& error) override;

   // void showCurrentTime() override;
    void showCurrentUser(const std::string& user) override;
    void handleResize();
};

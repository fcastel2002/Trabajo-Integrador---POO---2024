#include "CLIMessageView.h"
#include <iostream>

#include <curses.h> // Incluimos PDCurses para la visualización interactiva

// Muestra un mensaje estándar al usuario
void CLIMessageView::mostrarMensaje(const std::string& mensaje) {
    clear(); // Limpia la pantalla
    mvprintw(1, 1, "Mensaje: %s", mensaje.c_str());
    refresh(); // Actualiza la pantalla
    getch();   // Espera a que el usuario presione una tecla para continuar
}

// Muestra un error al usuario
void CLIMessageView::mostrarError(const std::string& error) {
    clear(); // Limpia la pantalla
    attron(A_BOLD | A_REVERSE);  // Resalta el mensaje de error
    mvprintw(1, 1, "Error: %s", error.c_str());
    attroff(A_BOLD | A_REVERSE); // Quita el resaltado
    refresh();
    getch();   // Espera a que el usuario presione una tecla para continuar
}

// Muestra la respuesta del servidor
void CLIMessageView::mostrarRespuesta(const std::string& respuesta) {
    clear(); // Limpia la pantalla
    mvprintw(1, 1, "Respuesta del servidor: %s", respuesta.c_str());
    refresh();
    getch();   // Espera a que el usuario presione una tecla para continuar
}

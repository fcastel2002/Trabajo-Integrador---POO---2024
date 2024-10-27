#include "CLIMessageView.h"
#include <iostream>
#include <curses.h> // Incluimos PDCurses para la visualización interactiva
#include <regex>

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
void CLIMessageView::mostrarRespuesta(XmlRpcValue& respuesta) {
    clear(); // Limpia la pantalla

    // Obtener el XML de la respuesta
    std::string xml = respuesta.toXml();

    // Expresión regular para encontrar el contenido dentro de <value>...</value>
    std::regex re("<value>(.*?)</value>");
    std::smatch match;

    mvprintw(1, 1, "Respuesta del servidor:");
    int line = 2;

    // Buscar y mostrar todos los valores dentro de <value>...</value>
    std::string::const_iterator searchStart(xml.cbegin());
    while (std::regex_search(searchStart, xml.cend(), match, re)) {
        std::string value = match[1].str();
        // Limpiar etiquetas internas si existen
        value = std::regex_replace(value, std::regex("<[^>]*>"), "");
        mvprintw(line++, 1, "%s", value.c_str());
        searchStart = match.suffix().first;
    }

    // Si no se encontraron valores, mostrar el XML completo
    if (line == 2) {
        mvprintw(2, 1, "Respuesta del servidor (sin parsear): %s", xml.c_str());
    }

    refresh();
    getch();   // Espera a que el usuario presione una tecla para continuar
}

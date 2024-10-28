#include "CLIMessageView.h"
#include <iostream>
#include <curses.h> // Incluimos PDCurses para la visualización interactiva
#include <regex>

// Muestra un mensaje estándar al usuario
void CLIMessageView::mostrarMensaje(std::string& mensaje) {
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
// Solicita al usuario su nombre de usuario y contraseña
std::pair<std::string, std::string> CLIMessageView::loguearse() {
    initscr();
    clear();
    echo(); // Muestra lo que se escribe

    char usuario[100];
    char contrasena[100];

    mvprintw(1, 1, "Ingrese usuario: ");
    getstr(usuario);

    mvprintw(2, 1, "Ingrese contraseña: ");
    getstr(contrasena);

    noecho(); // Deja de mostrar lo que se escribe
    refresh();

    return { usuario, contrasena };
}

// Solicita al usuario la IP y el puerto
std::pair<std::string, int> CLIMessageView::conectarse() {
    initscr();
    clear();
    echo(); // Muestra lo que se escribe

    char ip[100];
    char puerto[100];

    mvprintw(1, 1, "Ingrese IP: ");
    getstr(ip);

    mvprintw(2, 1, "Ingrese puerto: ");
    getstr(puerto);

    noecho(); // Deja de mostrar lo que se escribe
    refresh();

    return { ip, std::stoi(puerto) };
}
#include "PantallaCurses.h"
#include "ErrorHandler.h"
#include "curses.h"

PantallaCurses::PantallaCurses() {
    initscr();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    if (has_key(KEY_RESIZE)) {
        resize_term(0, 0);
    }
}

PantallaCurses::~PantallaCurses() {
    endwin();
}

int PantallaCurses::mostrarMenu(const std::vector<std::string>& opciones, const std::string& tituloMenu) {
    int seleccion = 0;
    int n_opciones = opciones.size();

    while (true) {
        limpiarPantalla();
        mvprintw(1, 5, tituloMenu.c_str());
        mvprintw(2, 1, "====================================");
        for (int i = 0; i < n_opciones; ++i) {
            if (i == seleccion) {
                attron(A_REVERSE);
            }
            mvprintw(i + 3, 2, opciones[i].c_str());
            attroff(A_REVERSE);
        }

        refrescarPantalla();
        int entrada = getch();

        switch (entrada) {
        case KEY_UP:
            seleccion = (seleccion == 0) ? n_opciones - 1 : seleccion - 1;
            break;
        case KEY_DOWN:
            seleccion = (seleccion == n_opciones - 1) ? 0 : seleccion + 1;
            break;
        case 10:
            return seleccion;
        }
    }
}

void PantallaCurses::limpiarPantalla() {
    clear();
    refresh();
}

void PantallaCurses::refrescarPantalla() {
    refresh();
}

void PantallaCurses::mostrarTexto(const std::string& mensaje) {
    limpiarPantalla();
    mvprintw(1, 1, "%s", mensaje.c_str());
    refrescarPantalla();
    getch();
}

std::string PantallaCurses::capturarEntrada(const std::string& mensaje, ErrorHandler& errorHandler) {
    limpiarPantalla();
    mvprintw(1, 1, "%s", mensaje.c_str());
    char buffer[80];
    echo();
    try {
        getstr(buffer);
    }
    catch (const std::exception& e) {
        errorHandler.handleException(e);
        noecho();
        return "";
    }
    noecho();
    return std::string(buffer);
}

std::vector<std::string> PantallaCurses::capturarEntradaMultiple(std::string& mensaje, ErrorHandler& errorHandler) {
    limpiarPantalla();
    std::vector<std::string> contenido;
    try {
        Archivo archivo_gcode(mensaje, "");
        if (!archivo_gcode.leer(errorHandler)) {
            errorHandler.logError("No se pudo leer el archivo", ErrorLevel::ERROR);
        }
        else {
            contenido = archivo_gcode.getContenido();
        }
    }
    catch (const std::exception& e) {
        errorHandler.handleException(e);
    }
    return contenido;
}

void PantallaCurses::mostrarError(const std::string& error) {
    limpiarPantalla();
    attron(A_BOLD | A_REVERSE);
    mvprintw(1, 1, "Error: %s", error.c_str());
    attroff(A_BOLD | A_REVERSE);
    refrescarPantalla();
    getch();
}

std::string PantallaCurses::capturarEleccion(const std::string& mensaje, const std::vector<std::string>& opciones, ErrorHandler& errorHandler) {
    limpiarPantalla();
    int seleccion;
    try {
        seleccion = mostrarMenu(opciones, mensaje.c_str());
    }
    catch (const std::exception& e) {
        errorHandler.handleException(e);
        return "";
    }
    return opciones[seleccion];
}

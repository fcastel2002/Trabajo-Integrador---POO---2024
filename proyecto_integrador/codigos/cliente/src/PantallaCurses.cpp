#include "PantallaCurses.h"
#include "curses.h"
#include "ErrorHandler.h"
#include <csignal>
#include <cstdlib>
#include <sstream>

/**
 * Constructor de la clase PantallaCurses.
 * Inicializa la pantalla y configura los colores y el teclado.
 */
PantallaCurses::PantallaCurses() {
    system("MODE 100,30");
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_WHITE);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    curs_set(FALSE);
    keypad(stdscr, TRUE);
}

/**
 * Destructor de la clase PantallaCurses.
 * Finaliza la pantalla.
 */
PantallaCurses::~PantallaCurses() {
    endwin();
}

/**
 * Muestra un menú con opciones y permite al usuario seleccionar una.
 * @param opciones Vector de opciones a mostrar.
 * @param tituloMenu Título del menú.
 * @return Índice de la opción seleccionada o -1 si se presiona ESC.
 */
int PantallaCurses::mostrarMenu(const std::vector<std::string>& opciones, const std::string& tituloMenu) {
    int seleccion = 0;
    size_t n_opciones = opciones.size();
    int max_y, max_x;

    getmaxyx(stdscr, max_y, max_x);
    limpiarPantalla();
    int titulo_x = (max_x - static_cast<int>(tituloMenu.size())) / 2;
    int menu_x = (max_x - 51) / 2;

    while (true) {
        handleResize();
        attron(COLOR_PAIR(2));
        mvprintw(1, titulo_x, "%s", tituloMenu.c_str());
        attroff(COLOR_PAIR(2));
        mvprintw(2, menu_x, "===================================================");
        for (int i = 0; i < n_opciones; ++i) {
            int opcion_x = menu_x;
            if (i == seleccion) {
                attron(A_REVERSE);
            }
            mvprintw(i + 3, opcion_x, opciones[i].c_str());
            if (i == seleccion) {
                attroff(A_REVERSE);
            }
        }
        mvprintw(28, 1, "Presione ESC para volver");
        refrescarPantalla();
        int entrada = getch();

        switch (entrada) {
        case KEY_UP:
            seleccion = (seleccion == 0) ? n_opciones - 1 : seleccion - 1;
            break;
        case KEY_DOWN:
            seleccion = (seleccion == n_opciones - 1) ? 0 : seleccion + 1;
            break;
        case 27:
            return -1;
        case 10:
            return seleccion;
        }
    }
}

/**
 * Limpia la pantalla.
 */
void PantallaCurses::limpiarPantalla() {
    clear();
    refresh();
}

/**
 * Refresca la pantalla.
 */
void PantallaCurses::refrescarPantalla() {
    refresh();
}

/**
 * Muestra un mensaje de texto en la pantalla.
 * @param mensaje Mensaje a mostrar.
 */
void PantallaCurses::mostrarTexto(const std::string& mensaje) {
    limpiarPantalla();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    std::istringstream stream(mensaje);
    std::string linea;
    int fila = 1;

    // Divide el mensaje en líneas y las imprime
    while (std::getline(stream, linea)) {
        if (fila >= max_y) break;
        mvprintw(fila, 1, "%s", linea.c_str());
        fila++;
    }

    refrescarPantalla();
    getch();
}

/**
 * Captura la entrada del usuario después de mostrar un mensaje.
 * @param mensaje Mensaje a mostrar.
 * @return Entrada del usuario como una cadena.
 */
std::string PantallaCurses::capturarEntrada(const std::string& mensaje) {
    limpiarPantalla();

    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    char buffer[80];
    int ch;
    int i = 0;
    int mensaje_len = static_cast<int>(mensaje.size());

    while (true) {
        handleResize();
        move(1, 1 + mensaje_len);
        mvprintw(1, 1, "%s", mensaje.c_str());
        refresh();
        mvprintw(28, 1, "Presione ESC para cancelar la operacion");

        ch = getch();
        if (ch == ERR) {
            continue;
        }
        if (ch == '\n') {
            break;
        }
        if (ch == 27) {
            nodelay(stdscr, TRUE);
            int next_ch = getch();
            nodelay(stdscr, FALSE);

            if (next_ch == ERR) {
                return "ESC";
            }
            else {
                ungetch(next_ch);
            }
        }
        if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (i > 0) {
                i--;
                mvaddch(1, 1 + mensaje_len + i, ' ');
                move(1, 1 + mensaje_len + i);
                refresh();
            }
        }
        else {
            if (i < sizeof(buffer) - 1) {
                buffer[i++] = ch;
                mvaddch(1, 1 + mensaje_len + i - 1, ch);
                refresh();
            }
        }
    }

    buffer[i] = '\0';

    return std::string(buffer);
}

/**
 * Muestra un mensaje de error en la pantalla.
 * @param error Mensaje de error a mostrar.
 */
void PantallaCurses::mostrarError(const std::string& error) {
    limpiarPantalla();
    attron(COLOR_PAIR(3));
    mvprintw(1, 1, "%s", error.c_str());
    attroff(COLOR_PAIR(3));
    refrescarPantalla();
    getch();
}

/**
 * Captura la entrada del usuario después de mostrar un mensaje y un menú de opciones.
 * @param mensaje Mensaje a mostrar.
 * @param opciones Vector de opciones a mostrar.
 * @return Entrada del usuario como una cadena.
 */
std::string PantallaCurses::capturarEntrada(const std::string& mensaje, const std::vector<std::string>& opciones) {
    limpiarPantalla();
    int seleccion = mostrarMenu(opciones, mensaje.c_str());
    if (seleccion == -1) {
        return "ESC";
    }
    return opciones[seleccion];
}

/**
 * Maneja el redimensionamiento de la ventana.
 */
void PantallaCurses::handleResize() {
    int new_y, new_x;
    getmaxyx(stdscr, new_y, new_x);

    if (is_termresized()) {
        resize_term(new_y, new_x);
        clear();
        refresh();
    }
}

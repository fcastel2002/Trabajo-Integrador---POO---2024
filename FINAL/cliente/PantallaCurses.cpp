#include "PantallaCurses.h"
#include "curses.h"
#include "ErrorHandler.h"
#include <csignal>
#include <cstdlib>

PantallaCurses::PantallaCurses() {
    system("MODE 80,25");
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);
    bkgd(COLOR_PAIR(1));
    curs_set(FALSE);         // Oculta el cursor
    keypad(stdscr, TRUE);    // Habilita teclas especiales (como las flechas)

    
}

PantallaCurses::~PantallaCurses() {
    endwin();
}

int PantallaCurses::mostrarMenu(const std::vector<std::string>& opciones, const std::string& tituloMenu) {
    int seleccion = 0;
    size_t n_opciones = opciones.size();
    int max_y, max_x;

        getmaxyx(stdscr, max_y, max_x);
        limpiarPantalla();
        int titulo_x = (max_x - static_cast<int>(tituloMenu.size())) / 2;
    while (true) {
        handleResize();
        attron(COLOR_PAIR(2)); 
        mvprintw(1, titulo_x, "%s", tituloMenu.c_str());
        attroff(COLOR_PAIR(2)); 
        mvprintw(2, (max_x - 36) / 2, "====================================");
        for (int i = 0; i < n_opciones; ++i) {
            size_t opcion_x = (max_x - opciones[i].size()) / 2;
            if (i == seleccion) {
                attron(A_REVERSE);
            }
            mvprintw(i + 3, opcion_x, opciones[i].c_str());
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
		case 27:  
			return -1;
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

std::string PantallaCurses::capturarEntrada(const std::string& mensaje) {
    limpiarPantalla();
    mvprintw(1, 1, "%s", mensaje.c_str());
    mvprintw(25, 25, "Presione ESC para cancelar la operacion");
    char buffer[80];
    echo();
    int ch;
    int i = 0; 
    while ((ch = getch()) != '\n') { 
        if (ch == 27) { // ESC key 
            noecho(); 
            return "ESC"; 
        }
        buffer[i++] = ch; 
        //addch(ch); 
    }
    buffer[i] = '\0'; 
    noecho(); 
    return std::string(buffer); 
}
void PantallaCurses::mostrarError(const std::string& error) {
    limpiarPantalla();
    attron(A_BOLD | A_REVERSE);  // Resalta el mensaje de error
    mvprintw(1, 1, "Error: %s", error.c_str());
    attroff(A_BOLD | A_REVERSE);
    refrescarPantalla();
    getch();  // Espera a que el usuario presione una tecla para continuar
}

std::string PantallaCurses::capturarEntrada(const std::string& mensaje, const std::vector<std::string>& opciones) {
    limpiarPantalla();
    int seleccion = mostrarMenu(opciones, mensaje.c_str());
    if (seleccion == -1) {
        return "ESC";
    }
    return opciones[seleccion];
}

void PantallaCurses::handleResize() {
    int new_y, new_x;
    getmaxyx(stdscr, new_y, new_x);
    if (is_termresized()) {
        resize_term(new_y, new_x);
        clear();
        refresh();
    }
}
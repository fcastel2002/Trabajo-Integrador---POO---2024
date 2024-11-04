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
	init_pair(3, COLOR_RED, COLOR_WHITE);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
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
    int menu_x = (max_x - 36) / 2; // Ajustar la posición del menú

    while (true) {
        handleResize();
		showCurrentUser(user_);
        attron(COLOR_PAIR(2));
        mvprintw(1, titulo_x, "%s", tituloMenu.c_str());
        attroff(COLOR_PAIR(2));
        mvprintw(2, menu_x, "====================================");
        for (int i = 0; i < n_opciones; ++i) {
            int opcion_x = menu_x; // Usar la misma posición de inicio para todas las opciones
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
	//showCurrentUser(user_);
	
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

    keypad(stdscr, TRUE);    // Activar teclas especiales
    cbreak();                // Desactivar el buffering de línea
    noecho();                // Desactivar el eco de caracteres

    char buffer[80];
    int ch;
    int i = 0;
    int mensaje_len = static_cast<int>(mensaje.size());

    while ((ch = getch()) != '\n') {
        handleResize();
        if (ch == 27) { // Posible tecla ESC
            // Verificar si es una secuencia de escape o la tecla ESC sola
            nodelay(stdscr, TRUE); // No bloquear getch()
            int next_ch = getch();
            nodelay(stdscr, FALSE); // Restaurar bloqueo de getch()

            if (next_ch == ERR) {
                // No hay más caracteres, es la tecla ESC sola
                return "ESC";
            }
            else {
                // Es una secuencia de otra tecla, devolver el carácter al buffer de entrada
                ungetch(next_ch);
            }
        }
        if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) { // Manejar la tecla de retroceso
            if (i > 0) {
                i--;
                mvaddch(1, 1 + mensaje_len + i, ' '); // Borrar el carácter de la pantalla
                move(1, 1 + mensaje_len + i);         // Mover el cursor hacia atrás
                refresh();                            // Actualizar la pantalla
            }
        }
        else {
            if (i < sizeof(buffer) - 1) { // Evitar desbordamiento del buffer
                buffer[i++] = ch;
                mvaddch(1, 1 + mensaje_len + i - 1, ch); // Mostrar el carácter en la pantalla
                refresh();                                // Actualizar la pantalla
            }
        }
    }

    buffer[i] = '\0';
    return std::string(buffer);
}
void PantallaCurses::mostrarError(const std::string& error) {
    limpiarPantalla();
    attron(COLOR_PAIR(3));  // Resalta el mensaje de error
    mvprintw(1, 1, "%s", error.c_str());
    attroff(COLOR_PAIR(3));
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
       // showCurrentUser(user_);
    }
}


void PantallaCurses::showCurrentUser(const std::string& user = "Ningun usuario ingresado") {
	user_ = user;
    
	
}
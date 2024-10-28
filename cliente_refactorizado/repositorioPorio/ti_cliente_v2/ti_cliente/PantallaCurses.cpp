#include "PantallaCurses.h"
#include "curses.h"

PantallaCurses::PantallaCurses() {
	initscr();
	curs_set(FALSE);         // Oculta el cursor
	keypad(stdscr, TRUE);    // Habilita teclas especiales (como las flechas)

	if (has_key(KEY_RESIZE)) {
		resize_term(0, 0);   // Ajusta el tamaño de la terminal a su valor actual
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

std::string PantallaCurses::capturarEntrada(const std::string& mensaje) {
	limpiarPantalla();
	mvprintw(1, 1, "%s", mensaje.c_str());
	char buffer[80];
	echo();
	getstr(buffer);
	noecho();

	
	return std::string(buffer);
}
std::vector<std::string> PantallaCurses::capturarEntradaMultiple(std::string& mensaje) {
	limpiarPantalla();
	
	
		Archivo archivo_gcode(mensaje,"");
		archivo_gcode.leer();
		return archivo_gcode.getContenido(); 

}

void PantallaCurses::mostrarError(const std::string& error) {
	limpiarPantalla();
	attron(A_BOLD | A_REVERSE);  // Resalta el mensaje de error
	mvprintw(1, 1, "Error: %s", error.c_str());
	attroff(A_BOLD | A_REVERSE);
	refrescarPantalla();
	getch();  // Espera a que el usuario presione una tecla para continuar
}


std::string PantallaCurses::capturarEleccion(const std::string& mensaje, const std::vector<std::string>& opciones) {
	limpiarPantalla();
	
	int seleccion = mostrarMenu(opciones,mensaje.c_str());
	return opciones[seleccion];
}
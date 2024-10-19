#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu(Cliente& cliente) : cliente(cliente) {}

// Inicializa la pantalla para PDCurses
void MainMenu::inicializarPantalla() {
    initscr();               // Inicializa PDCurses
    noecho();                // No muestra lo que se escribe
    curs_set(FALSE);         // Oculta el cursor
    keypad(stdscr, TRUE);    // Habilita teclas especiales (como las flechas)
}

// Termina el uso de PDCurses
void MainMenu::terminarPantalla() {
    endwin();                // Termina PDCurses
}

void MainMenu::setComandos(const std::vector<const std::string>& comandos) {
    m_comandos = comandos;
}

void MainMenu::mostrarMenu() {
    inicializarPantalla(); // Inicia la interfaz de PDCurses

    std::string opciones[] = {
        "Conectar al Robot",
        "Desconectar del Robot",
        "Activar Motores",
        "Desactivar Motores",
        "Mover Efector (con Velocidad)",
        "Mover Efector (solo Posición)",
        "Homming",
        "Ejecucion Automática",
        "Reportar Estado",
        "Reportar Posición Actual",
        "Cambiar a Modo Absoluto",
        "Cambiar a Modo Relativo",
        "Activar Efector",
        "Desactivar Efector",
        "Salir"
    };

    int seleccion = 0;           // Índice de la opción seleccionada
    int n_opciones = sizeof(opciones) / sizeof(opciones[0]);

    while (true) {
        clear();                 // Limpia la pantalla

        // Muestra el menú
        for (int i = 0; i < n_opciones; i++) {
            if (i == seleccion) {
                attron(A_REVERSE);  // Resalta la opción seleccionada
            }
            mvprintw(i + 1, 1, opciones[i].c_str()); // Imprime cada opción
            if (i == seleccion) {
                attroff(A_REVERSE); // Quita el resaltado
            }
        }
        refresh();               // Actualiza la pantalla

        // Captura la entrada del usuario
        int entrada = getch();    // Captura la tecla presionada
        switch (entrada) {
        case KEY_UP:          // Mover hacia arriba
            seleccion--;
            if (seleccion < 0) seleccion = n_opciones - 1;
            break;
        case KEY_DOWN:        // Mover hacia abajo
            seleccion++;
            if (seleccion >= n_opciones) seleccion = 0;
            break;
        case 10:              // Enter
            manejarSeleccion(seleccion);
			Orden my_order = crearOrden(manejarSeleccion(seleccion));
            cliente.enviarComando(my_order);
            if (seleccion == n_opciones - 1) { // Si es la opción "Salir"
                terminarPantalla();
                return;
            }
            break;
        }
    }
}

// Maneja la selección del menú
const std::string MainMenu::manejarSeleccion(int seleccion) {
    switch (seleccion) {
    case 0:
        return "conectar_robot";
    case 1:
		return "desconectar_robot";
    case 2:
		return "activar_motores";
    case 3:
		return "desactivar_motores";

    case 4:
		return "mover_efector_velocidad";
    case 5:
        
		return "mover_efector_posicion";
    case 6:
        
		return "homming";

	case 7:

		return "ejecucion_automatica";
	case 8:
            
		return "reportar_estado";
	case 9:

		return "reportar_posicion_actual";

	case 10:

		return "cambiar_modo_absoluto";

	case 11:

		return "cambiar_modo_relativo";

	case 12:

		return "activar_efector";

	case 13:

		return "desactivar_efector";

    case 14:
        cliente.getConsole().mostrarMensaje("Saliendo...\n");
        break;
    }
}


Orden MainMenu::crearOrden(const std::string& tipo) {
	std::vector <std::string> parametros;

	for (int i = 0; i < 3; i++) {
        clear();
        echo();
		mvprintw(1, 1, "Ingrese el parametro %d: ", i + 1);
		char buffer[100];
		getstr(buffer);
		std::string param{ buffer };
		if (param.empty()) {
            break;
		}
		parametros.push_back(param);
	}
    noecho();
	return Orden(tipo, parametros);
}
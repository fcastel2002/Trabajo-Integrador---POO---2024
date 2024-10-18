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
        "Ejecución Automática",
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
            if (seleccion == n_opciones - 1) { // Si es la opción "Salir"
                terminarPantalla();
                return;
            }
            break;
        }
    }
}

// Maneja la selección del menú
void MainMenu::manejarSeleccion(int seleccion) {
    switch (seleccion) {
    case 0:
        cliente.conectarServidor();
        break;
    case 1:
        
        break;
    case 2:
        cliente.enviarComando("activar_motores");
        break;
    case 3:
        cliente.enviarComando("desactivar_motores");
        break;
    case 4:
        cliente.enviarComando("mover_efector");
        break;
    case 5:
        cliente.enviarComando("mover_efector_posicion");
        break;
    case 6:
        cliente.enviarComando("homming");
        break;
    case 7:
        cliente.enviarComando("ejecutar_automatico");
        break;
    case 8:
        cliente.enviarComando("reportar_estado");
        break;
    case 9:
        cliente.enviarComando("reportar_posicion");
        break;
    case 10:
        cliente.enviarComando("modo_absoluto");
        break;
    case 11:
        cliente.enviarComando("modo_relativo");
        break;
    case 12:
        cliente.enviarComando("activar_efector");
        break;
    case 13:
        cliente.enviarComando("desactivar_efector");
        break;
    case 14:
        std::cout << "Saliendo...\n";
        break;
    }
}

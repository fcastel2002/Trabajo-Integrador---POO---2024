#include "MainMenu.h" 
#include <iostream>

MainMenu::MainMenu(Cliente& cliente) : cliente(cliente) {
    inicializarPantalla();
	Orden my_order = crearOrden("comandos");
	
    m_comandos = cliente.pedirComandos(my_order);
}

// Inicializa la pantalla para PDCurses
void MainMenu::inicializarPantalla() {
    initscr();               // Inicializa PDCurses
	refresh();			   // Refresca la pantalla
    noecho();                // No muestra lo que se escribe
    curs_set(FALSE);         // Oculta el cursor
    keypad(stdscr, TRUE);    // Habilita teclas especiales (como las flechas)

    // Habilita el redimensionamiento de la ventana
    



    if (has_key(KEY_RESIZE)) {
        resize_term(0, 0);   // Ajusta el tamaño de la terminal a su valor actual
    }
}

// Termina el uso de PDCurses
void MainMenu::terminarPantalla() {
    endwin();                // Termina PDCurses
}

void MainMenu::setComandos(const std::vector<std::string>& comandos) {
    m_comandos = comandos;
}

void MainMenu::mostrarMenu() {
                // Inicia la interfaz de PDCurses
    /*
    std::string opciones[] = {
        "Conectar al Robot",
        "Desconectar del Robot",
        "Activar Motores",
        "Desactivar Motores",
        "Mover Efector (con Velocidad)",
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
    */

    int seleccion = 0;           // Índice de la opción seleccionada
	int n_opciones = m_comandos.size(); // Número de opciones

    while (true) {
        clear();                 // Limpia la pantalla

        // Muestra el menú
        for (int i = 0; i < n_opciones; i++) {
            if (i == seleccion) {
                attron(A_REVERSE);  // Resalta la opción seleccionada
            }
            mvprintw(i + 1, 1, m_comandos[i].c_str()); // Imprime cada opción
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
        case KEY_RESIZE:      // Cambio de tamaño de ventana
            clear();          // Limpia la pantalla
            resize_term(0, 0); // Ajusta el tamaño de la terminal
            break;
        case 10:              // Enter
            if (seleccion == n_opciones - 1) { // Si es la opción "Salir"
                terminarPantalla();
                return;
            }
            manejarSeleccion(seleccion);
            Orden my_order = crearOrden(manejarSeleccion(seleccion));
            cliente.enviarComando(my_order);

            break;
        }
    }
}

// Maneja la selección del menú
const std::string MainMenu::manejarSeleccion(int seleccion) {
    if (seleccion >= 0 && seleccion < m_comandos.size()) {
        return m_comandos[seleccion];
    }
    return "";
}

Orden MainMenu::crearOrden(const std::string& comando) {
    std::vector <std::vector<std::string>> parametros(4);
    for (auto& vec : parametros) {
        vec.resize(1); // Inicializa cada vector interno con un tamaño de 1
    }
    parametros[0][0] = cliente.getUser();
    parametros[1][0] = cliente.getPass();
    parametros[2][0] = comando;
 
    if (parametros[2][0] == "conectar") {
        const int cantidad = 2;
        clear();
        echo();
        std::vector<std::string> param(cantidad);

        for (int i = 0; i < cantidad; i++) {
            if (i == 0) mvprintw(1, 1, "Ingrese puerto COM: ");
            else mvprintw(2, 1, "Ingrese Baudrate: ");
            char buffer[100];
            getstr(buffer);
            param[i] = buffer;
        }

        parametros[3] = param; // Convertimos XmlRpcValue a string para almacenarlo en parametros
    }
    else if (parametros[2][0] == "mover_efector") {
        const int cantidad = { 4 };
        clear();
        echo();
        std::vector<std::string> param(cantidad);
        for (int i = 0; i < cantidad; i++) {
            if (i == 0) mvprintw(1, 1, "Ingrese X: ");
            else if (i == 1) mvprintw(2, 1, "Ingrese Y: ");
            else if (i == 2) mvprintw(3, 1, "Ingrese Z: ");
            else mvprintw(4, 1, "Ingrese Velocidad: ");
            char buffer[100];
            getstr(buffer);
			param[i] = buffer;
        }
		    parametros[3] = param;  

    }
  
    else {
        parametros[3] = std::vector<std::string>();
    }

    return Orden(parametros);
}

class PantallaCurses
!!!134957.cpp!!!	PantallaCurses()

    initscr();
    curs_set(FALSE);         // Oculta el cursor
    keypad(stdscr, TRUE);    // Habilita teclas especiales (como las flechas)

    if (has_key(KEY_RESIZE)) {
        resize_term(0, 0);   // Ajusta el tamaño de la terminal a su valor actual
    }
!!!135085.cpp!!!	~PantallaCurses()

    endwin();
!!!135213.cpp!!!	mostrarMenu(in opciones : std::vector<std::string>, in tituloMenu : std::string) : int

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
!!!135341.cpp!!!	mostrarTexto(in mensaje : std::string) : void

    limpiarPantalla();
    mvprintw(1, 1, "%s", mensaje.c_str());
    refrescarPantalla();
    getch();
!!!135469.cpp!!!	limpiarPantalla() : void

    clear();
    refresh();
!!!135597.cpp!!!	capturarEntrada(in mensaje : std::string) : std::string

    limpiarPantalla();
    mvprintw(1, 1, "%s", mensaje.c_str());
    char buffer[80];
    echo();
    getstr(buffer);
    noecho();

    return std::string(buffer);
!!!135725.cpp!!!	archivoToVector(inout mensaje : std::string) : std::vector<std::string>

    limpiarPantalla();
    ErrorHandler errorHandler;

    try {
        Archivo archivo_gcode(mensaje, "");
        if (!archivo_gcode.abrir()) {
            errorHandler.logError("No se pudo abrir el archivo: " + mensaje, ErrorLevel::ERROR);
            mostrarError("No se pudo abrir el archivo: " + mensaje);
            return {};
        }

        if (!archivo_gcode.leer()) {
            errorHandler.logError("Error al leer el archivo: " + mensaje, ErrorLevel::ERROR);
            mostrarError("Error al leer el archivo: " + mensaje);
            return {};
        }

        return archivo_gcode.getContenido();

    }
    catch (const std::exception& e) {
        errorHandler.handleException(e);
        mostrarError("Excepcion al procesar el archivo.");
        return {};
    }
!!!135853.cpp!!!	capturarEleccion(in mensaje : std::string, in opciones : std::vector<std::string>) : std::string

    limpiarPantalla();
    int seleccion = mostrarMenu(opciones, mensaje.c_str());
    return opciones[seleccion];
!!!135981.cpp!!!	refrescarPantalla() : void

    refresh();
!!!136109.cpp!!!	mostrarError(in error : std::string) : void

    limpiarPantalla();
    attron(A_BOLD | A_REVERSE);  // Resalta el mensaje de error
    mvprintw(1, 1, "Error: %s", error.c_str());
    attroff(A_BOLD | A_REVERSE);
    refrescarPantalla();
    getch();  // Espera a que el usuario presione una tecla para continuar

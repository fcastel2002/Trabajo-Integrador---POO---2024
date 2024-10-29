class MainMenu
!!!133165.cpp!!!	mostrarMenu() : void

    m_pantalla->refrescarPantalla();
    while (m_flagMenu) {
        int seleccion = m_pantalla->mostrarMenu(m_opcionesCliente, "Bienvenido al menu principal");
        std::string opcion = procesarSeleccionLocal(seleccion);

        if (opcion == "exit") {
            break;
        }
        else if (opcion == "login") {
            cliente.login();
        }
        if (m_comandos.size()>2) {

            if (opcion == "rpc") {
                while (true) {
                    int seleccion = m_pantalla->mostrarMenu(m_opciones, "Menu de comandos");
                    if (!procesarSeleccion(seleccion)) {
                        break;
                    }
                }
            }
        }
    }
!!!133293.cpp!!!	setComandos() : void

    OrdenBuilder builder;
    builder.conUsuario(cliente.getUser())
        .conClave(cliente.getPass())
        .conComando("comandos");

    ErrorHandler errorHandler;

    try {
        Orden ordenComandos = builder.build();
        m_comandos = cliente.pedirComandos(ordenComandos);

        m_opciones = m_comandos;
        m_opciones.push_back("Cerrar sesion");
        m_opciones.push_back(m_opcionesCliente[2]);
    }
    catch (const std::exception& e) {
        errorHandler.handleException(e);
        m_pantalla->mostrarError("Error al configurar comandos.");
    }
!!!133421.cpp!!!	procesarSeleccion(in seleccion : int) : bool

    std::string comando = manejarSeleccion(seleccion, "servidor");
    OrdenBuilder builder;
    ErrorHandler errorHandler;

    if (comando == "Salir") {
        m_flagMenu = false;
        return false;
    }

    if (comando == "Cerrar sesion") {
        return false;
    }

    builder.conUsuario(cliente.getUser())
        .conClave(cliente.getPass())
        .conComando(comando);

    std::vector<std::string> mensajesParametros = builder.obtenerEtiquetasParametros();
    std::vector<std::string> parametros;

    try {
        if (comando == "Ejecutar automatico") {
            std::string nombreArchivo = m_pantalla->capturarEntrada("Ingrese el nombre del archivo:");
            std::string choice = m_pantalla->capturarEleccion("�Desea enviar el archivo? (s/n)", { "Si", "No" });
            parametros.push_back(nombreArchivo);

            if (choice == "Si") {
                m_pantalla->mostrarTexto("Opcion: " + choice);

                std::vector<std::string> entradas = m_pantalla->archivoToVector(nombreArchivo);
                for (const auto& entrada : entradas) {
                    m_pantalla->mostrarTexto(entrada);
                    parametros.push_back(entrada);
                }
            }
            else {
                parametros.push_back("");
            }
        }
        else {
            for (const auto& mensaje : mensajesParametros) {
                parametros.push_back(m_pantalla->capturarEntrada(mensaje));
            }
        }

        builder.conParametros(parametros);
        Orden orden = builder.build();

        m_pantalla->mostrarTexto("Comando enviado: " + comando);
        for (size_t i = 0; i < parametros.size(); ++i) {
            m_pantalla->mostrarTexto("Parametro " + std::to_string(i) + " enviado: " + parametros[i]);
        }

        cliente.enviarComando(orden);

    }
    catch (const std::exception& e) {
        errorHandler.handleException(e);
        m_pantalla->mostrarError("Error al procesar la seleccion.");
    }

    return true;
!!!133549.cpp!!!	procesarSeleccionLocal(in seleccion : int) : std::string

    std::string comando = manejarSeleccion(seleccion, "cliente");
    if (comando == "Salir") {
        m_flagMenu = false;
        return "exit";
    }
    if (comando == "Login") {
        return "login";
    }
    if (comando == "Mostrar comandos") {
        setComandos();
        if (m_comandos.empty()) {

        }
        return "rpc";
    }
    return "";
!!!133677.cpp!!!	manejarSeleccion(in seleccion : int, in para : std::string) : std::string

    if (para == "servidor" && seleccion >= 0 && seleccion < m_opciones.size()) {
        return m_opciones[seleccion];
    }
    else if (para == "cliente" && seleccion >= 0 && seleccion < m_opcionesCliente.size()) {
        return m_opcionesCliente[seleccion];
    }
    return "";

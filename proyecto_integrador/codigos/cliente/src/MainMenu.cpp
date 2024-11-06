#include "MainMenu.h" 
#include "ErrorHandler.h"
#include <iostream>

using namespace std;

/**
 * Constructor de la clase MainMenu.
 * @param cliente Referencia a un objeto Cliente para la interacción con el servidor.
 * @param pantalla Puntero a una interfaz de pantalla para la interacción con el usuario.
 */
MainMenu::MainMenu(Cliente& cliente, IPantalla* pantalla)
    : cliente(cliente), m_pantalla(pantalla) {}

/**
 * Configura los comandos disponibles obteniéndolos del servidor.
 */
void MainMenu::setComandos() {
    OrdenBuilder builder;
    builder.conUsuario(cliente.getUser())
        .conClave(cliente.getPass())
        .conComando("comandos");

    ErrorHandler errorHandler;

    try {
        Orden ordenComandos = builder.build();
        m_comandos = cliente.pedirComandos(ordenComandos);

        m_opciones = m_comandos;
        m_opciones.push_back("Volver");
        m_opciones.push_back(m_opcionesCliente[2]);
    }
    catch (const std::exception& e) {
        errorHandler.handleException(e);
        m_pantalla->mostrarError("Error al configurar comandos.");
    }
}

/**
 * Muestra el menú principal y maneja la interacción del usuario.
 */
void MainMenu::mostrarMenu() {
    m_pantalla->refrescarPantalla();
    while (m_flagMenu) {
        int seleccion = m_pantalla->mostrarMenu(m_opcionesCliente, "Bienvenido al menu principal");
        std::string opcion = procesarSeleccion(seleccion, "local");

        if (opcion == "exit") {
            break;
        }
        else if (opcion == "login") {
            cliente.login();
            m_opcionesCliente[0] = std::string("Cerrar sesion: ") + cliente.getUser();

        }
        else if (opcion == "cerrar sesion") {
            cliente.cerrarSesion();
            m_opcionesCliente[0] = "Login";
        }
        else
            if (m_comandos.size() > 2) {

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
}

/**
 * Procesa la selección del usuario en el menú.
 * @param seleccion Índice de la opción seleccionada.
 * @param quien Contexto de la selección (local o servidor).
 * @return Una cadena que indica la acción a realizar.
 */
std::string MainMenu::procesarSeleccion(int seleccion, const std::string& quien) {
    std::string comando = manejarSeleccion(seleccion, "cliente");
    if (comando == "Salir") {
        m_flagMenu = false;
        return "exit";
    }
    if (comando == "Login") {
        return "login";
    }

    if (comando.find("Cerrar sesion") != std::string::npos) {
        return "cerrar sesion";
    }
    if (comando == "Mostrar comandos") {
        setComandos();
        if (m_comandos.empty()) {
            return "";
        }
        return "rpc";
    }
    return "";
}

/**
 * Maneja la selección del usuario en el menú.
 * @param seleccion Índice de la opción seleccionada.
 * @param para Contexto de la selección (cliente o servidor).
 * @return Una cadena que indica el comando seleccionado.
 */
const std::string MainMenu::manejarSeleccion(int seleccion, const std::string& para) {
    if (para == "servidor" && seleccion >= 0 && seleccion < m_opciones.size()) {
        return m_opciones[seleccion];
    }
    else if (para == "servidor" && seleccion == -1) {
        return "Volver";
    }
    else if (para == "cliente" && seleccion >= 0 && seleccion < m_opcionesCliente.size()) {
        return m_opcionesCliente[seleccion];
    }
    return "";
}

/**
 * Procesa la selección del usuario en el menú de comandos del servidor.
 * @param seleccion Índice de la opción seleccionada.
 * @return true si se debe continuar mostrando el menú, false en caso contrario.
 */
bool MainMenu::procesarSeleccion(int seleccion) {
    std::string comando = manejarSeleccion(seleccion, "servidor");
    OrdenBuilder builder;
    ErrorHandler errorHandler;

    if (comando == "Salir") {
        m_flagMenu = false;
        return false;
    }

    if (comando == "Cerrar sesion") {
        cliente.cerrarSesion();
        return false;
    }
    if (comando == "Volver" || comando == "ESC") {
        return false;
    }


    builder.conUsuario(cliente.getUser())
        .conClave(cliente.getPass())
        .conComando(comando);

    std::vector<std::string> mensajesParametros = builder.obtenerEtiquetasParametros();
    std::vector<std::string> parametros;

    try {
        if (comando == "Ejecutar automatico") {
            std::string nombreArchivo,choice,ruta;
            std::string dedonde = m_pantalla->capturarEntrada("Donde esta el archivo que desea ejecutar?: ", { "Servidor","Cliente" });
            if (dedonde == "Cliente") {
                ruta = "./"+m_pantalla->capturarEntrada("Ingrese la ruta (sugerido: archivos_gcode): ")+"/";
                nombreArchivo = m_pantalla->capturarEntrada("Seleccione el archivo para la ejecucion automatica:", Archivo::obtenerArchivos(ruta)); //metodo static necesario
                choice = m_pantalla->capturarEntrada("Desea enviar el archivo? (s/n)", { "Si", "No" });

            }
            else {
                std::string nombreArchivo = m_pantalla->capturarEntrada("Ingrese el nombre del archivo presente en el servidor (sin extension): ");
            }

            if (nombreArchivo == "ESC") return true;
            if (choice == "ESC") return true;
            Archivo archivo_gcode(nombreArchivo, ruta);
            parametros.push_back(nombreArchivo);

            if (choice == "Si") {
               ;

                std::vector<std::string> entradas = archivo_gcode.archivoToVector(archivo_gcode);
                for (const auto& entrada : entradas) {
                    
                    parametros.push_back(entrada);
                }
            }
            else {
                parametros.push_back("");
            }
        }
        else if (comando == "Aprendizaje") {
            std::string nombreArchivo = m_pantalla->capturarEntrada("Ingrese el nombre del archivo:");
            std::string choice = m_pantalla->capturarEntrada("Que quiere hacer con el aprendizaje?: ", mensajesParametros);
            parametros.push_back(nombreArchivo);
            parametros.push_back(choice);
        }
        else if (comando == "Actuar efector") {
            std::string choice = m_pantalla->capturarEntrada("Desea activar o desactivar el efector?", mensajesParametros);
            parametros.push_back(choice);
        }

        else {
            for (const auto& mensaje : mensajesParametros) {
                parametros.push_back(m_pantalla->capturarEntrada(mensaje));
            }
        }
        if (std::find(parametros.begin(), parametros.end(), "ESC") != parametros.end()) {
            return true;
        }
        builder.conParametros(parametros);
        Orden orden = builder.build();

        /*
        m_pantalla->mostrarTexto("Comando enviado: " + comando);

        for (size_t i = 0; i < parametros.size(); ++i) {
            m_pantalla->mostrarTexto("Parametro " + std::to_string(i) + " enviado: " + parametros[i]);
        }
        */
        cliente.enviarComando(orden);

    }
    catch (const std::exception& e) {
        errorHandler.handleException(e);
        m_pantalla->mostrarError("Error al procesar la seleccion.");
    }

    return true;
}

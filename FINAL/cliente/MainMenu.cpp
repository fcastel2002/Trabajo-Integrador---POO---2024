#include "MainMenu.h"
#include "ErrorHandler.h"
#include <iostream>

using namespace std;

MainMenu::MainMenu(Cliente& cliente, IPantalla* pantalla, ErrorHandler& errorHandler) : cliente(cliente), m_pantalla(pantalla)  {
    OrdenBuilder builder;
    builder.conUsuario(cliente.getUser())
           .conClave(cliente.getPass())
           .conComando("comandos");
    Orden ordenComandos = builder.build();

    try {
        m_comandos = cliente.pedirComandos(ordenComandos, errorHandler);
    } catch (const std::exception& e) {
        errorHandler.handleException(e);
    }
}

// Inicializa la pantalla para PDCurses
void MainMenu::setComandos(const std::vector<std::string>& comandos) {
    m_comandos = comandos;
}

void MainMenu::mostrarMenu(ErrorHandler& errorHandler) {
    m_pantalla->refrescarPantalla();
    while (true) {
        int seleccion = m_pantalla->mostrarMenu(m_comandos, "Bienvenido al menu principal");
        if (!procesarSeleccion(seleccion, errorHandler)) {
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

bool MainMenu::procesarSeleccion(int seleccion, ErrorHandler& errorHandler) {
    std::string comando = manejarSeleccion(seleccion);
    OrdenBuilder builder;
    if (comando == "Salir") {
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
            std::string choice = m_pantalla->capturarEleccion("Desea enviar el archivo? (s/n)", {"Si", "No"});
            parametros.push_back(nombreArchivo);

            if (choice == "Si") {
                std::vector<std::string> entradas = m_pantalla->capturarEntradaMultiple(nombreArchivo);
                for (const auto& entrada : entradas) {
                    parametros.push_back(entrada);
                }
            }
        } else {
            for (const auto& mensaje : mensajesParametros) {
                parametros.push_back(m_pantalla->capturarEntrada(mensaje));
            }
        }
    } catch (const std::exception& e) {
        errorHandler.handleException(e);
        return false;
    }

    builder.conParametros(parametros);
    Orden orden = builder.build();

    if (!cliente.enviarComando(orden, errorHandler)) {
        errorHandler.logError("Error al enviar el comando al servidor.", ErrorLevel::ERROR);
        return false;
    }

    return true;
}
#include "MainMenu.h" 
#include <iostream>

using namespace std;

MainMenu::MainMenu(Cliente& cliente, IPantalla* pantalla) : cliente(cliente), m_pantalla(pantalla)  {
    OrdenBuilder builder;
	builder.conUsuario(cliente.getUser())
           .conClave(cliente.getPass())
           .conComando("comandos");
	Orden ordenComandos = builder.build();
	m_comandos = cliente.pedirComandos(ordenComandos);
    //m_pantalla->mostrarTexto(m_comandos[3]);

}

// Inicializa la pantalla para PDCurses

void MainMenu::setComandos(const std::vector<std::string>& comandos) {
    m_comandos = comandos;
}


void MainMenu::mostrarMenu() {
    m_pantalla->refrescarPantalla();
    while (true) {
        int seleccion = m_pantalla->mostrarMenu(m_comandos, "Bienvenido al menu principal");
        if (!procesarSeleccion(seleccion)) {
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

bool MainMenu::procesarSeleccion(int seleccion) {
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
    builder.conParametros(parametros);
    Orden orden = builder.build();
    cliente.enviarComando(orden);

    return true;
}

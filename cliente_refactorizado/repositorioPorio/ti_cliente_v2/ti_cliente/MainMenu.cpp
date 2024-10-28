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

}

// Inicializa la pantalla para PDCurses

void MainMenu::setComandos(const std::vector<std::string>& comandos) {
    m_comandos = comandos;
}


void MainMenu::mostrarMenu() {
    m_pantalla->refrescarPantalla();
	int seleccion = m_pantalla->mostrarMenu(m_comandos);
	procesarSeleccion(seleccion);
}

// Maneja la selección del menú
const std::string MainMenu::manejarSeleccion(int seleccion) {
    if (seleccion >= 0 && seleccion < m_comandos.size()) {
        return m_comandos[seleccion];
    }
    return "";
}

void MainMenu::procesarSeleccion(int seleccion) {
	std::string comando = manejarSeleccion(seleccion);
    OrdenBuilder builder;

    builder.conUsuario(cliente.getUser())
           .conClave(cliente.getPass())
		   .conComando(comando);
    std::vector<std::string> mensajesParametros = builder.obtenerEtiquetasParametros();
    std::vector<std::string> parametros;
    for (const auto& mensaje : mensajesParametros) {
        parametros.push_back(m_pantalla->capturarEntrada(mensaje));
    }
    builder.conParametros(parametros);
    Orden orden = builder.build();
	cliente.enviarComando(orden);
}

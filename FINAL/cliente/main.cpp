#include "Cliente.h"
#include "PantallaCurses.h"
#include "MainMenu.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "XmlRpc.h"



int main() {
    try {
        PantallaCurses pantalla;

        Cliente cliente(pantalla);
        MainMenu menu(cliente, &pantalla);
        menu.mostrarMenu();
    }
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
    }

    return 0;
}

//
// Created by Pachi on 09/10/2024.
//

#include "MainMenu.h"

#include <iostream>
#include <ostream>

void MainMenu::mostrarMenu() {
    system("cls");
    std::cout << "=======MENU======" << std::endl;
    for(const auto& opcion : opciones) {
        std::cout << opcion << std::endl;
    }
    std::cout << "Seleccione una opcion" << std::endl;
}
void MainMenu::seleccionarOpcion(int opcion) {
    switch(opcion) {
        case 1:
            cliente ->conectar();
            break;
        case 2:
            std::string nombreUsuario,clave;
        std::cout << "Ingrese el nombre del usuario: ";
        std::cin >> nombreUsuario;
        std::cout << "Ingrese clave: ";
        std::cin >> clave;
        cliente -> autenticar(nombreUsuario, clave);
        break;
        case 3:
            std::string tipo,parametros;
            

    }
}

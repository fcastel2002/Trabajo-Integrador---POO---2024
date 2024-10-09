//
// Created by Pachi on 09/10/2024.
//

#ifndef MAINMENU_H
#define MAINMENU_H

#include <vector>
#include "Cliente.h"

class MainMenu {
private:
    Cliente* cliente;
    std::vector<std::string> opciones;
public:
    MainMenu(Cliente* Cliente) : cliente(Cliente) {
        opciones = {
        "1. Conectar al servidor",
        "2. Iniciar sesion",
        "3. Enviar comando",
        "4. Subir archivo G-Code",
        "5. Desconectar del servidor",
        "6. Salir"
        };
    };
    void mostrarMenu();
    void seleccionarOpcion(int opcion);

};



#endif //MAINMENU_H

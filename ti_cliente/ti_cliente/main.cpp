#include <iostream>
#include "Cliente.h"
#include "CLIMessageView.h"
#include "MainMenu.h"

int main() {
    CLIMessageView messages;

    Cliente cliente("127.0.0.1", 9000, messages);

    MainMenu menu(messages);

    menu.addOption(1, "Activar motores", "activar_motores");
    menu.addOption(2, "Desactivar motores", "desactivar_motores");
    menu.addOption(3, "Homming", "homming");

    while (true) {
        std::optional<std::string> cmdOption = menu.displayMenu(); 
        if (!cmdOption.has_value()) {
            messages.showMessage("Saliendo del programa", MessageType::INFO);
            break;
        }

        std::string comando = cmdOption.value();
        bool success = cliente.enviarComando(comando);
        if (success) {
            messages.showMessage("Comando ejecutado correctamente", MessageType::INFO);
        } else {
            messages.showMessage("Error al ejecutar el comando", MessageType::ERROR);
        }
    }
    return 0;
}

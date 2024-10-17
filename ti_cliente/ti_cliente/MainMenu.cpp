#include "MainMenu.h"
#include <limits>
#include <iostream>

MainMenu::MainMenu(IMessageView& messageView) : m_messageView{ messageView } {}

void MainMenu::addOption(int optionNumber, std::string descripcion, std::string comando) {
    MenuOption option{ optionNumber, descripcion, comando };
    m_options.push_back(option);
    m_messageView.showMessage("Opción agregada: " + std::to_string(optionNumber) + " - " + descripcion, MessageType::INFO);
}

std::optional<std::string> MainMenu::displayMenu() {
    m_messageView.showMessage("Número de opciones: " + std::to_string(m_options.size()), MessageType::INFO);
    int selectedOption;

    while (true) {
        m_messageView.showMessage("------------------------------------");
        m_messageView.showMessage("------------Menu principal----------\n", MessageType::INFO);

        if (m_options.empty()) {
            m_messageView.showMessage("No hay opciones disponibles\n", MessageType::ERROR);
            return std::nullopt;
        }

        for (const auto& option : m_options) {
            m_messageView.showMessage(std::to_string(option.optionNumber) + ". " + option.descripcion + "\n", MessageType::INFO);
        }

        m_messageView.showMessage("0: Salir\n", MessageType::INFO);
        m_messageView.showMessage("Seleccione una opcion: ", MessageType::INFO);

        std::cin >> selectedOption;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            m_messageView.showMessage("Opcion invalida\n", MessageType::ERROR);
            continue;
        }
        if (selectedOption == 0) {
            return std::nullopt;
        }

        bool found = false;
        for (const auto& option : m_options) {
            if (option.optionNumber == selectedOption) {
                found = true;
                return option.comando;
            }
        }
        m_messageView.showMessage("Opcion invalida, intente de nuevo\n", MessageType::ERROR);
    }
}

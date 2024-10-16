#include "MainMenu.h"
#include <limits>
#include <iostream>
MainMenu::MainMenu(IMessageView& messageView) : m_messageView{ messageView } {}

void MainMenu::addOption(int optionNumber, std::string descripcion, std::string comando) {
	MenuOption option{ optionNumber, descripcion, comando };
	m_options.push_back(option);
}

std::optional<std::string> MainMenu::displayMenu() {
	int selectedOption;

	while (true) {
		m_messageView.showMessage("Menu principal\n", MessageType::INFO);
		
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
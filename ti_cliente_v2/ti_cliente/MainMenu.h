#pragma once
#include <vector>
#include <string>
#include <functional>

#include<optional>

#include "IMessageView.h"

struct MenuOption {
	int optionNumber;
	std::string descripcion;
	std::string comando;
};
class MainMenu
{
public: 
	MainMenu(IMessageView& messageView);
	void addOption(int optionNumber, std::string descripcion, std::string comando);	
	std::optional<std::string> displayMenu();

private:
	std::vector<MenuOption> m_options;
	IMessageView& m_messageView;	
};


#include "CLIMessageView.h"

#include <iostream>


void CLIMessageView::showMessage(const std::string& message, MessageType type)
{
	switch (type)
	{
	case MessageType::INFO:
		std::cout << "[INFO]: " << message << std::endl;
		break;
	case MessageType::ERROR:
		std::cerr << "[ERROR]: " << message << std::endl;
		break;
	case MessageType::WARNING:
		std::cout << "[WARNING]: " << message << std::endl;
		break;
	default:
		break;
	}
}
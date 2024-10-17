#pragma once
#include "IMessageView.h"
class CLIMessageView :
    public IMessageView 
{
public:
	void showMessage(const std::string& message, MessageType type = MessageType::INFO) override;
};


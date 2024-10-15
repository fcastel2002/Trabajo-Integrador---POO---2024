#pragma once
#include <string>

enum class MessageType {
	INFO,
	ERROR,
	WARNING
};

class IMessageView {
public:
	virtual ~IMessageView() = default;
	virtual void showMessage(const std::string& message, MessageType type = MessageType::INFO) override;
};

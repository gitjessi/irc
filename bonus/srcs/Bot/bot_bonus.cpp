#include "../../include/Bot_bonus.hpp"

Bot::Bot() : Client(-1)
{
	setNickname("Bot");
	setUserName("Botman");
	setRealName("ChatBot");
}

Bot::~Bot() {}

std::string Bot::myMessage(const std::string &msg)
{
	if (msg.find("hello") != std::string::npos)
		return "Hello, I am Bot 👋";
	if (msg.find("ping") != std::string::npos)
		return "pong";
	else if (msg.find("help") != std::string::npos)
		return "Commands dispo: hello, help, ping";
	return "";
}

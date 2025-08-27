#include "../../include/IRC.hpp"

int registredClient(std::vector<std::string> &parts, Client &client, std::string &password, std::string &command, std::vector<Client *> &clients)
{
	if (command == "bp1")
	{
		client.setNickname("Saiisako", clients, client);
		client.setUserName("Sacha");
		client.setRealName("Salut !");
		client.setRegistredNick();
		client.setRegistredUser(true);
		client.setRegistredPassWord();
	}
	// else if (command == "bp2")
	// {
	// 	client.setNickname("Jess", clients, client);
	// 	client.setUserName("Jessica");
	// 	client.setRealName("Bonjour");
	// 	client.setRegistredNick();
	// 	client.setRegistredUser(true);
	// 	client.setRegistredPassWord();
	// }
	// else if (command == "bp3")
	// {
	// 	client.setNickname("LeBonSushi", clients, client);
	// 	client.setUserName("Maxence");
	// 	client.setRealName("Yolo");
	// 	client.setRegistredNick();
	// 	client.setRegistredUser(true);
	// 	client.setRegistredPassWord();
	// }
	if (command != "PASS" && client.getRegistredPassWord() == false)
	{
		client.sendReply(ERR_UNKNOWNCOMMAND(command));
		return 1;
	}
	if (command == "PASS")
		if (goToPass(password, parts, client) == false)
			return 1;
	if (!client.getRegistredPassWord())
	{
		client.sendReply(ERR_NOTREGISTERED);
		return 1;
	}
	if (command != "NICK" && command != "USER" && command != "PASS" && command != "bp1" && command != "bp2" && command != "bp3")
	{
		client.sendReply(ERR_NOTREGISTERED);
		return 1;
	}
	if (command == "NICK")
		if (!goToNickName(parts, client, clients))
			return 1;
	if (command == "USER")
		if (!goToUser(parts, client))
			return 1;
	return 0;
}

std::vector<std::string> cut_to_string(const std::vector<std::string> &parts, int flag)
{
	std::vector<std::string> result;

	if (flag < 0 || flag >= static_cast<int>(parts.size()))
		return result;

	for (size_t i = flag; i < parts.size(); ++i) {
		if (!parts[i].empty() && parts[i][0] == ':') {
			std::string trailing;
			for (size_t j = i; j < parts.size(); ++j) {
				if (!trailing.empty())
					trailing += " ";
				trailing += parts[j];
			}
			result.push_back(trailing);
			break;
		} else {
			result.push_back(parts[i]);
		}
	}

	return result;
}

int executeCommand(std::string &line, Client &client, std::string password, std::vector<Channel *> &channels, std::vector<Client *> &clients)
{
	std::cout << client << std::endl;
	std::vector<std::string> parts = split(line, ' ');

	for (size_t i = 0; i != parts.size(); i++)
	{
		if (parts[i].find("\r\n") != std::string::npos)
			parts[i].erase(parts[i].find("\r\n"));
		else if (parts[i].find("\n") != std::string::npos)
			parts[i].erase(parts[i].find("\n"));
		else if (parts[i].find("\r") != std::string::npos)
			parts[i].erase(parts[i].find("\r"));
		if (parts[i].empty())
			return 1;
	}
	std::string command = parts[0];
	if (command == "CAP" && parts[1] == "LS")
		return 2;
	if (!client.isReadyToRegister())
	{
		if (registredClient(parts, client, password, command, clients) == 1)
			return 1;
	}
	else if (command != "NICK" && command != "JOIN" && command != "MODE" && command != "privmsg" && command != "PRIVMSG" && command != "INVITE" && command != "TOPIC" && command != "KICK" && command != "kick" && client.isReadyToRegister())
	{
		client.sendReply(ERR_UNKNOWNCOMMAND(command));
		return 2;
	}
	else if (command == "JOIN")
	{
		if (!goToJoin(parts, client, channels, clients))
			return 1;
	}
	else if (command == "NICK")
	{
		if (!goToNickName(parts, client, clients))
			return 1;
	}
	else if (command == "MODE")
	{
		if (!goToMode(parts, client, channels, clients))
			return 1;
	}
	else if (command == "KICK" || command == "kick")
	{
		if (parts.size() < 3)
		{
			client.sendReply(ERR_NEEDMOREPARAMS(client.getNickName()));
			return 1;
		}
		std::vector<std::string> arguments = cut_to_string(parts, 1);
		if (!goToKick(arguments, client, channels, clients))
			return 1;
	}
	else if (command == "PRIVMSG" || command == "privmsg")
	{
		if (parts.size() < 3)
		{
			client.sendReply(ERR_NEEDMOREPARAMS(client.getNickName()));
			return 1;
		}
		std::vector<std::string> arguments = cut_to_string(parts, 1);
		if (!goToPrivMsg(arguments, client, channels, clients))
			return 1;
	}
	else if (command == "INVITE")
	{
		if (!goToInvite(parts, client, channels, clients))
			return 1;
	}
	else if (command == "TOPIC")
	{
		if (!goToTopic(parts, client, channels))
			return 1;
	}
	return 0;
}

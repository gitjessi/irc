#include "../../include/IRC_bonus.hpp"

bool goToPass(std::string &password, std::vector<std::string> &parts, Client &client)
{
	
	if (client.isReadyToRegister())
	{
		client.sendReply(ERR_ALREADYREGISTRED);
		return false;
	}
	if (parts.size() < 2)
	{
		client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
		return false;
	}
	std::string passwordClient = parts[1];
	std::cout << "password serveur = " << password << std::endl;
	std::cout << "password client = " << passwordClient << std::endl;

	if (password != passwordClient)
	{
		client.sendReply(ERR_PASSWDMISMATCH);
		return false;
	}
	client.setRegistredPassWord();
	std::cout << client << std::endl;
	return true;
}

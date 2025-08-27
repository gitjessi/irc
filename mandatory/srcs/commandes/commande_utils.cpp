#include "../../include/Commande.hpp"
#include "../../include/Client.hpp"
#include "../../include/IRC.hpp"

std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &v)
{
	for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); ++it)
		os << *it << std::endl;
	return os;
}

void print_channel(Client &client, Channel *channel)
{
	std:: cout << std::endl;
	std:: cout << "_client = " << client.getNickName() << std::endl;
	std:: cout << "_channel = " << channel->getChannel() << std::endl;
	std:: cout << "_name_operator = " << channel->getOperator() << std::endl;
	std:: cout << "_key_channel = " << channel->getKey() << std::endl;
	std:: cout << "_inviteOnly_active = " << channel->inviteOnlyIsActive() << std::endl;
	std:: cout << "_topic_active = " << channel->TopicOperatorIsActive() << std::endl;
	std:: cout << "_passWord_channel_ative = " << channel->isPassorWord() << std::endl;
	std:: cout << "_limiteUsersInChannel = " << channel->getLimiteUserChannel() << std::endl;
	std:: cout << "_limiteUserIsActive = " << channel->isLimiteUserIsActive() << std::endl;
	std:: cout << "_countUsersChanne = " << channel->getCountUserChannel() << std::endl;
	std:: cout << "_nameTopic = " << channel->getNameTopic() << std::endl;\
	std:: cout << "_user_liste_channel = " << channel->getUserList() << std::endl;
	std:: cout << std::endl;
}


#include "../../include/IRC_bonus.hpp"

// INVITE Bob #42
bool goToInvite(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients)
{
	if (parts.size() < 3)
	{
		client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
		return (false);
	}
	std::string name_user = parts[1];
	std::string name_channel = parts[2];
	Channel *chan = NULL;
	if (name_channel[0] != '#')
	{
		client.sendReply(ERR_NOSUCHCHANNEL(name_channel));
		return (false);
	}
	for (unsigned int i = 0; i < channels.size(); i++)
	{
		if (channels[i]->getChannel() == name_channel)
		{
			chan = channels[i];
			break ;
		}
	}
	if (chan)
	{
		if (!chan->isOperator(client.getNickName()))
		{
			client.sendReply(ERR_CHANOPRIVSNEEDED(client.getNickName(), name_channel));
			return (false);
		}

		if (chan->hasClient(name_user) == true)
		{
			std::cout << "dans hasclient " << std::endl;
			client.sendReply(ERR_USERONCHANNEL(client.getServerName(), client.getNickName(), name_user, name_channel));
			return (false);
		}

		// Vérifier si l’utilisateur est déjà invité
		if (chan->isInvited(name_user))
		{
			return (true);
		}
		if (chan->isInChannel(name_user))
		{
			std::cout << "dans isschannel " << std::endl;
			client.sendReply(ERR_USERONCHANNEL(client.getServerName(), client.getNickName(), name_user, name_channel));
			return (false);
		}

		// Ajoute l'invitation
		chan->addInvite(name_user);
	}
	if (!chan)
	{
		client.sendReply(ERR_NOSUCHCHANNEL(name_channel));
		return (false);
	}
	Client *targetClient = NULL;
	for (unsigned int i = 0; i < clients.size(); i++)
	{
		if (clients[i]->getNickName() == name_user)
		{
			targetClient = clients[i];
			break ;
		}
	}
	if (!targetClient)
	{
		client.sendReply(ERR_NOSUCHNICK(client.getServerName(), client.getNickName()));
		return (false);
	}
	if (targetClient)
	{
		targetClient->sendReply(":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " INVITE " + name_user + " :" + name_channel);
		client.sendReply(RPL_INVITING(client.getServerName() , client.getNickName(), name_user, name_channel));
	}
	print_channel(client, chan);
	return (true);
}

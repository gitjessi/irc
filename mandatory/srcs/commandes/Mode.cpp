#include "../../include/IRC.hpp"


static bool verif_line(std::string &name_channel, std::string &mode, Client &client)
{
	(void)mode;
	if (name_channel.empty() && name_channel[0] != '#')
	{
		client.sendReply(ERR_BADCHANNAME(client.getNickName(), name_channel));
		return false;
	}
	if (mode.empty() || mode.size() != 2)
	{
		client.sendReply(ERR_UNKNOWNMODE(mode));
		return false;
	}
	if ((mode[0] != '+' && mode[0] != '-') || (mode[1] != 'i' && mode[1] != 't' && mode[1] != 'k' && mode[1] != 'o' && mode[1] != 'l'))
	{
		client.sendReply(ERR_UNKNOWNMODE(mode));
		return false;
	}
	return true;
}


static bool verif_parametre(std::vector<std::string> parts, std::string &parametre, Client &client, std::string &name_channel)
{
	if (parametre.empty())
	{
		client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
		return false;
	}
	for (unsigned int i = 0; i < parametre.size(); i++)
	{
		if (!isdigit(parametre[i]))
		{
			client.sendReply(ERR_INVALIDLIMIT(name_channel));
			return false;
		}
	}
	return true;
}

bool goToMode(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients)
{
	std::string parametre;
	(void)clients;

	if (parts.size() < 3)
	{
		client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
		return false;
	}
	std::string name_channel = parts[1];
	std::string mode = parts[2];

	if (!verif_line(name_channel, mode, client))
		return false;

	if (parts.size() == 4)
		parametre = parts[3];

	Channel *targetChannel = NULL;
	for (unsigned int i = 0; i < channels.size(); i++)
	{
		if (channels[i]->getChannel() == name_channel)
		{
			targetChannel = channels[i];
			break;
		}
	}

	if (!targetChannel)
	{
		client.sendReply(ERR_NOSUCHCHANNEL(name_channel));
		return false;
	}

	if (!targetChannel->isOperator(client.getNickName()))
	{
		client.sendReply(ERR_CHANOPRIVSNEEDED(client.getNickName(), name_channel));
		return false;
	}
	verif_line(name_channel, mode, client);

	bool active = (mode[0] == '+');

	std::string modeMsg = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " MODE " + targetChannel->getChannel() + " " + mode;


	switch (mode[1])
	{
		case 'i': // invite only
			if (parts.size() != 3)
			{
				client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
				return false;
			}
			targetChannel->setInviteOnly(active);
			targetChannel->broadcast(modeMsg, client);
			client.sendReply(modeMsg);
			break;

		case 't': // topic change restreint aux opérateurs
			if (parts.size() != 3)
			{
				client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
				return false;
			}
			targetChannel->setTopicOperator(active);
			targetChannel->broadcast(modeMsg, client);
			client.sendReply(modeMsg);
			break;

		case 'k': // mot de passe
			if (active)
			{
				if (parts.size() != 4)
				{
					client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
					return false;
				}
				targetChannel->setKey(parametre);
				targetChannel->setPassWord(true);
				targetChannel->broadcast(modeMsg + " " + parametre, client);
				client.sendReply(modeMsg + " " + parametre);
			}
			else
			{
				targetChannel->setPassWord(false);
				targetChannel->broadcast(modeMsg + " " + parametre, client);
				client.sendReply(modeMsg + " " + parametre);
			}
			break;

		case 'o': // ajout/retrait opérateur
			if (parts.size() != 4)
			{
				client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
				return false;
			}
			if (active)
			{
				targetChannel->addOperator(parametre);
				targetChannel->broadcast(modeMsg + " " + parametre, client);
				client.sendReply(modeMsg + " " + parametre);
			}
			else
			{
				targetChannel->removeOperator(parametre);
				targetChannel->broadcast(modeMsg, client);
				client.sendReply(modeMsg);
			}
			break;

		case 'l': // limite d’utilisateurs
			if (active)
			{
				if (parts.size() != 4)
				{
					client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
					return false;
				}
				verif_parametre(parts, parametre, client, name_channel);
				int limite = atoi(parametre.c_str());
				targetChannel->setLimiteUserChannel(limite);
				targetChannel->setLimiteUserIsActive(true);
				targetChannel->broadcast(modeMsg + " " + parametre, client);
				client.sendReply(modeMsg + " " + parametre);
			}
			else
			{
				targetChannel->setLimiteUserIsActive(false);
				targetChannel->broadcast(modeMsg, client);
				client.sendReply(modeMsg);
			}
			break;

		default:
			client.sendReply(ERR_UNKNOWNMODE(std::string(1, mode[1])));
			return false;
	}

	client.sendReply(":server 324 " + client.getNickName() + " " +
				targetChannel->getChannel() + " " + targetChannel->getModesAsString());

	print_channel(client, targetChannel);
	return true;
}
// La commande /join te permet d'aller sur un channel. Il ne faut pas oublier le # pour un channel du réseau, le & pour un channel du serveur, ou enfin le +. De plus si le channel est protégé par une clef (mode +k), vous devez spécifier la clef après le nom du channel. Si le channel n'existe pas, un channel de ce nom sera créé et tu deviendra automatiquement opérateur.
// Syntaxe : /join <channel> [clef]
// Example : /join #cool
// Example : /join #franco_cool COOLCLEF

#include "../../include/IRC_bonus.hpp"

// client join a channel
bool goToJoin(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients, Bot &bot)
{
	(void)clients;
	bool found_channel = false;
	if (parts.size() < 2)
	{
		client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
		return false;
	}
	if (!client.isReadyToRegister())
	{
		client.sendReply(ERR_NOTREGISTERED);
		return false;
	}
	std::string name_channel = parts[1];
	std::string namechannel = name_channel;
	if (parts.size() > 2)
		std::string key = parts[2];

	if (name_channel[0] != '#' && name_channel[0] != '&')
	{
		client.sendReply(ERR_BADCHANNAME(client.getNickName(), name_channel));
		return false;
	}
	for (unsigned int i = 0; namechannel[i]; i++)
	{
		if (namechannel[i] == ',' || namechannel[i] == ':')
		{
			client.sendReply(ERR_BADCHANNAME(client.getNickName(), name_channel));
			return false;
		}
	}

	// search channel if exist
	for (unsigned int i = 0; i < channels.size(); i++)
	{
		if (channels[i]->getChannel() == namechannel)
		{
			Channel *chan = channels[i];
			if (chan->isPassorWord())
			{
				if (parts.size() < 2 || chan->getKey() != parts[2])
				{
					client.sendReply(ERR_BADCHANNELKEY(namechannel));
					return false;
				}
			}
			if (chan->inviteOnlyIsActive() == 1)
			{
				if (!chan->userIsListeInvite(client.getNickName()))
				{
					client.sendReply(ERR_INVITEONLYCHAN(client.getServerName(), client.getNickName(), namechannel));
					return false;
				}
			}
			if (chan->isLimiteUserIsActive() && chan->getCountUserChannel() > chan->getLimiteUserChannel())
			{
				client.sendReply(ERR_CHANNELISFULL(namechannel));
				return false;
			}
			client.joinChannel(chan);
			chan->addClient(client);
			client.sendReply(":server 353 " + client.getNickName() + " = " + chan->getChannel() + " :" + chan->getUserList());
			std::cout << "CLIENT NICKNAME IN JOIN = [" << client.getNickName() << "]" << std::endl;
			print_channel(client, chan);
			chan->broadcast(":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " JOIN :" + chan->getChannel(), client);

			found_channel = true;
			client.sendReply(":server 366 " + client.getNickName() + ' ' + chan->getChannel() + " :End of /NAMES list.");
			break;
		}
	}
	// channel no found
	if (!found_channel)
	{
		Channel *newChannel = new Channel(namechannel);
		newChannel->addClient(client);
		newChannel->addClient(bot);
		if (parts.size() > 2)
		{
			std::string key = parts[2];
			newChannel->setKey(key);
			newChannel->setPassWord(true);
		}
		std::cout << newChannel->getKey() << std::endl;
		newChannel->addOperator(client.getNickName());
		newChannel->setOperator(client.getNickName());
		client.sendReply(":server 353 " + client.getNickName() + " = " + newChannel->getChannel() + " :" + newChannel->getUserList());
		print_channel(client, newChannel);
		channels.push_back(newChannel);
		client.joinChannel(newChannel);
		client.sendReply(":server 366 " + client.getNickName() + ' ' + newChannel->getChannel() + " :End of /NAMES list.");
	}

	std::cout << client.getNickName() << " has joined the channel " << namechannel << std::endl;
	return true;
}

// Code	Nom	Signification
// 403	ERR_NOSUCHCHANNEL	Le canal n’existe pas
// 405	ERR_TOOMANYCHANNELS	Le client a rejoint trop de canaux
// 471	ERR_CHANNELISFULL	Le canal est plein
// 472	ERR_UNKNOWNMODE ou ERR_INVITEONLYCHAN	Le canal est en mode "invite-only"
// 475	ERR_BADCHANNELKEY	Mauvais mot de passe (clé) pour rejoindre
// 332	RPL_TOPIC	Le serveur envoie le topic du canal après avoir rejoint

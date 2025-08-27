#include "../../include/Commande_bonus.hpp"
#include "../../include/IRC_bonus.hpp"
#include "../../include/Error_bonus.hpp"

// This command allow you to send a message to a channel, or directly to someone.
// It work like this :
// PRIVMSG <receiver(s)>, :<message>

std::pair<bool, Channel *> verifChan(const std::string &value, std::vector<Channel *> &channels)
{
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getChannel() == value)
			return std::make_pair(true, *it);
	}
	return std::pair<bool, Channel *>(false, NULL);
}

std::pair<bool, Client *> verifClient(const std::string &value, std::vector<Client *> &Clients)
{
	for (std::vector<Client *>::iterator it = Clients.begin(); it != Clients.end(); ++it)
	{
		if ((*it)->getNickName() == value)
			return std::make_pair(true, *it);
	}
	return std::pair<bool, Client *>(false, NULL);
}

void sendToClients(std::string &msg, std::vector<Client *> &clientToSend, Client &client)
{
	for (std::vector<Client *>::iterator it = clientToSend.begin(); it != clientToSend.end(); ++it)
	{
		std::ostringstream msg_pattern;
		msg_pattern << ":" << client.getNickName() << "!" << client.getUserName() << "@localhost " << "PRIVMSG " << client.getNickName() << " " << msg << "\r\n";
		std::string final_msg = msg_pattern.str();
		send((*it)->getFd(), final_msg.c_str(), final_msg.size(), 0);
	}
}

void	sendToChannel(std::string &msg, std::vector<Channel *> &channelToSend, Client &client)
{
	std::cout << "entering send to chan" << std::endl;
	for (std::vector<Channel *>::iterator it = channelToSend.begin(); it != channelToSend.end(); ++it)
	{
		for (std::vector<Client *>::iterator it2 = (*it)->getUserListV().begin(); it2 != (*it)->getUserListV().end(); ++it2)
		{
			if ((*it2)->getNickName() == client.getNickName())
				continue ;
			std::cout << "entering here idk what to say" << std::endl;
			std::ostringstream msg_pattern;
			msg_pattern << ":" << client.getNickName() << "!" << client.getUserName() << "@localhost " << "PRIVMSG " << (*it)->getChannel() << " " << msg << "\r\n";
			std::string final_msg = msg_pattern.str();
			send((*it2)->getFd(), final_msg.c_str(), final_msg.size(), 0);
		}
	}
}

void	removeDuplicates(int flag, std::vector<Channel *> &channelsToSend, std::vector<Client *> &clientsToSend)
{
	if (flag == 0)
	{
		for (std::vector<Client *>::iterator it = clientsToSend.begin(); it != clientsToSend.end(); ++it)
		{
			for (std::vector<Client *>::iterator it2 = it + 1; it2 != clientsToSend.end(); ++it2)
			{
				if ((*it)->getNickName() == (*it2)->getNickName())
					clientsToSend.erase(it2);
			}
		}
	}
	else
	{
		for (std::vector<Channel *>::iterator it = channelsToSend.begin(); it != channelsToSend.end(); ++it)
		{
			for (std::vector<Channel *>::iterator it2 = it + 1; it2 != channelsToSend.end(); ++it2)
			{
				if ((*it)->getChannel() == (*it2)->getChannel())
					channelsToSend.erase(it2);
			}
		}
	}
}

void	verif_right(std::vector<Channel *> &channels, Client &client)
{
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (!(*it)->hasClient(client.getNickName()))
		{
			client.sendReply(ERR_CANNOTSENDTOCHAN(client.getNickName(), (*it)->getChannel()));
			channels.erase(it);
			continue ;
		}
	}
	return ;
}

bool goToPrivMsg(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients)
{
	std::cout << "entering privmsg function" << std::endl;
	std::cout << parts << std::endl;
	static bool flag_channel = false;
	static bool flag_nick = false;
	std::vector<Client *> clientToSend;
	std::vector<Channel *> chanToSend;

	if (parts[1][0] != ':')
	{
		client.sendReply(ERR_NOTEXTTOSEND(client.getNickName()));
		return (true);
	}
	std::vector<std::string> receivers = split(parts[0], ',');
	for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end(); ++it)
	{
		if (it->empty())
		continue;
		// if it's a channel
		if ((*it)[0] == '#')
		{
			std::pair<bool, Channel *> vc = verifChan(*it, channels);
			if (vc.first)
			{
				chanToSend.push_back(vc.second);
				continue;
			}
			else
			{
				if (!flag_channel)
				{
					std::cout << "entering here" << std::endl;
					client.sendReply(ERR_NOSUCHCHANNEL(*it));
					flag_channel = true;
				}
			}
		}
		// if its a client
		std::pair<bool, Client *> vn = verifClient(*it, clients);
		if (vn.first)
		{
			clientToSend.push_back(vn.second);
		}
		else
		{
			if (!flag_nick)
			{
				client.sendReply(ERR_NOSUCHNICK(client.getServerName(), *it));
				flag_nick = true;
			}
		}
	}

	if (!clientToSend.empty())
	{
		removeDuplicates(0, chanToSend, clientToSend);
		sendToClients(parts[1], clientToSend, client);
	}

	if (!chanToSend.empty())
	{
		removeDuplicates(1, chanToSend, clientToSend);
		verif_right(chanToSend, client);
		if (!chanToSend.empty())
			sendToChannel(parts[1], chanToSend, client);
	}

	// Reset pour prochain appel
	flag_nick = false;
	flag_channel = false;

	return false;
}

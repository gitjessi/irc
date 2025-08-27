#include "../../include/Commande_bonus.hpp"
#include "../../include/IRC_bonus.hpp"
#include "../../include/Error_bonus.hpp"

bool	DoesClientExistOnServer(std::vector<Client *> &clients, std::string &nick)
{
	bool	found = false;
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickName() == nick)
			return (true);
	}
	if (!found)
		return (false);
	return (false);
}

bool DoesClientExistOnChan(std::vector<Client *> &clients, std::vector<Channel *> &channels, const std::string &nick)
{
	Client *tmp = NULL;

	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickName() == nick)
		{
			tmp = *it;
			break;
		}
	}
	if (!tmp)
		return (false);

	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->hasClient(tmp->getNickName()))
			return (true);
	}

	return (false);
}

Client& searchClient(Channel &chan, const std::string &nickToSearch)
{
	std::vector<Client *>& clients = chan.getClient();
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickName() == nickToSearch)
			return **it;
	}
	throw std::runtime_error("Client not found");
}


// KICK <channel> <user> [<comment>]
bool goToKick(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients)
{
	std::cout << parts << std::endl;
	std::cout << parts.size() << std::endl;

	bool	allowedToSend = false;
	if (channels.size() == 0)
	{
		client.sendReply(ERR_NOSUCHCHANNEL(parts[0]));
		return (false);
	}
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getChannel().find(parts[0]) != std::string::npos)
		{
			if ((*it)->getOperator().find(client.getNickName()) != std::string::npos)
			{
				allowedToSend = true;
				break ;
			}
			else
			{
				client.sendReply(ERR_CHANOPRIVSNEEDED(client.getNickName(), (*it)->getChannel()));
				return (false);
			}
		}
	}
	if (allowedToSend == true)
	{
		if (DoesClientExistOnServer(clients, parts[1]) == true)
		{
			if (parts[1] == client.getNickName())
			{
				client.sendReply("Cannot auto kick :)");
				return (false);
			}
			if (DoesClientExistOnChan(clients, channels, parts[1]) == true)
			{
				for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
				{
					if ((*it)->getChannel() == parts[0])
					{
						Channel* chanPtr = *it;
						Client& targetClient = searchClient(*chanPtr, parts[1]);
						chanPtr->removeClient(targetClient);
						std::string msg;
						if (parts.size() == 2)
							msg = ":server KICK " + parts[0] + " " + parts[1] + "\r\n";
						else if (parts.size() == 3)
							msg = ":server KICK " + parts[0] + " " + parts[1] + " " + parts[2] + "\r\n";
						send(targetClient.getFd(), msg.c_str(), msg.size(), 0);
						client.sendReply(":server 353 " + client.getNickName() + " = " + chanPtr->getChannel() + " :" + chanPtr->getUserList());
						client.sendReply(":server 366 " + client.getNickName() + ' ' + chanPtr->getChannel() + " :End of /NAMES list.");
						break;
					}
				}
				
			}
			else
			{
				client.sendReply(ERR_USERNOTINCHANNEL(parts[1], parts[0]));
				return (false);
			}
		}
		else
		{
			// client.sendReply(ERR_NOSUCHNICK(client.getServerName(), client.getNickName()));
			return (false);
		}
	}
	else
	{
		client.sendReply(ERR_NOSUCHCHANNEL(parts[0]));
		return (false);
	}

	return true;
}

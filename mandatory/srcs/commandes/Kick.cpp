/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 01:41:28 by skock             #+#    #+#             */
/*   Updated: 2025/08/27 18:24:17 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Commande.hpp"
#include "../../include/IRC.hpp"
#include "../../include/Error.hpp"

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
			client.sendReply(ERR_USERNOTINCHANNEL(parts[1], parts[0]));
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

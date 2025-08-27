/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_bonus.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:22:03 by skock             #+#    #+#             */
/*   Updated: 2025/08/26 16:02:09 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "Client_bonus.hpp"
#include "Channel_bonus.hpp"
#include "Commande_bonus.hpp"
#include "csignal"
#include "Bot_bonus.hpp"

class Server
{
private:
	int _socketFd;
	int _servPort;
	std::string _password;
	std::string _servName;
	std::vector<Client *> clients;
	std::vector<Channel *> channels;
	Bot _bot;

public:
	Server(std::string password, std::string port);
	~Server();

	// GET
	int getSocketFd();
	int getServPort();
	std::string getPassword();
	std::string getServName();
	// SET
	void setSocketFd(int fd);
	void setServPort(int port);
	void setPassword(std::string password);
	void setServName(std::string serverName);
	void boot();
	void run();
};

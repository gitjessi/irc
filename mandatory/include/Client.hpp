
// Client -> (envoie une ligne) -> Server -> analyse ligne -> Command
//-> appelle la bonne fonction

#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cerrno>
#include <set>
#include <vector>

class Channel;

class Client
{
	private:
		int						_fd;
		std::string				_nickname;
		std::string				_username;
		std::string				_realname;
		std::string				_hostname;
		std::string				_servername;
		bool					_registredNick;
		bool					_registredUser;
		bool					_registredPassWord;
		std::vector<Channel*>	_channels;

		public:
		Client(int fd);
		virtual ~Client();
		std::string accumulated;

		// GET
		const std::vector<Channel*>& getChannels() const;
		const std::string &getNickName() const;
		const std::string &getUserName() const;
		const std::string &getRealName() const;
		const std::string &getHostName() const;
		const std::string &getServerName() const;
		bool getRegistredNick() const;
		bool getRegistredUser() const;
		bool getRegistredPassWord() const;
		int getFd() const;

		// SET
		void setNickname(const std::string &nick, std::vector<Client *> clients, Client &client);
		void setNickname(const std::string &nick);
		void setUserName(const std::string &user);
		void setRealName(const std::string &realname);
		void setHostName(const std::string &hostname);
		void setServerName(const std::string &hostname);
		void setRegistredNick();
		void setRegistredUser(bool arg);
		void setRegistredPassWord();

		// GET INFORMATIONS
		bool isReadyToRegister() const;

		// ACTIONS
		void sendReply(const std::string &msg) const;
		void joinChannel(Channel* chan);
		bool operator==(const Client &other) const { return this->getFd() == other.getFd(); };
};

std::ostream &operator<<(std::ostream &flux, const Client &c);

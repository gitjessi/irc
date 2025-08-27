#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include "Client.hpp"

class Channel
{
	private:
		std::string _channel;
		std::string _name_operator;
		std::string _key_channel;
		std::vector<Client *> _clients;
		std::vector<std::string> _operators;
		std::set<std::string> _inviteUserInChannel;
		bool _inviteOnlyActif;
		bool _topicOperatorActif;
		bool _passWordActif;
		int _limiteUsersInChannel;
		bool _limiteUserIsActif;
		int _countUsersChannel;
		std::string _nameTopic;

	public:
		Channel();
		Channel(std::string &channel);
		~Channel();

		// ACTIONS
		int removeOperator(const std::string &name);
		void addClient(Client &client);
		void removeClient(Client &client);
		void broadcast(const std::string &msg, Client &client);
		void addOperator(const std::string &nickname);
		void addInvite(const std::string &name_invite);
		void addCountUserChannel();
		void removeCountUserChannel();

		// GET INFORMATIONS
		bool hasClient(const std::string &name);
		bool isInChannel(const std::string& nick) const;
		bool isOperator(const std::string &name);
		bool userIsListeInvite(const std::string &name);
		bool isInvited(const std::string &nick) const;
		bool TopicOperatorIsActive() const;
		bool isPassorWord() const;
		bool isLimiteUserIsActive() const;
		bool inviteOnlyIsActive() const;

		// GET
		std::string getChannel() const;
		std::string getUserList();
		std::vector<std::string> getOperatorV();
		std::string getOperator() const;
		std::vector<Client *>	 &getUserListV(void);
		std::vector<Client *> &getClient();
		int getLimiteUserChannel() const;
		int getCountUserChannel() const;
		std::string getNameTopic() const;
		std::string getModesAsString() const;
		std::string getKey() const;

		// SET
		void setPassWord(bool value);
		void setNameTopic(const std::string &topic);
		void setOperator(const std::string &name);
		void setInviteOnly(bool value);
		void setTopicOperator(bool value);
		void setLimiteUserChannel(const int limite);
		void setKey(const std::string &key);
		void setLimiteUserIsActive(bool value);
};
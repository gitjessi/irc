#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include "Client_bonus.hpp"

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

	std::string getChannel() const;

	void addClient(Client &client);
	void removeClient(Client &client);
	bool hasClient(const std::string &name);
	bool isInChannel(const std::string& nick) const;
	
	
	std::string getUserList();
	std::vector<Client *>	 &getUserListV(void);
	void broadcast(const std::string &msg, Client &client);
	
	std::vector<Client *> &getClient();
	std::vector<std::string> getOperatorV();
	std::string getOperator() const;
	void setOperator(const std::string &name);
	void addOperator(const std::string &nickname);
	bool isOperator(const std::string &name);
	int removeOperator(const std::string &name);

	bool inviteOnlyIsActive() const;
	void setInviteOnly(bool value);
	void addInvite(const std::string &name_invite);
	bool userIsListeInvite(const std::string &name);
	bool isInvited(const std::string &nick) const;

	bool TopicOperatorIsActive() const;
	void setTopicOperator(bool value);

	std::string getKey() const;
	void setKey(const std::string &key);

	void setPassWord(bool value);
	bool isPassorWord() const;

	void setLimiteUserIsActive(bool value);
	bool isLimiteUserIsActive() const;

	int getLimiteUserChannel() const;
	void setLimiteUserChannel(const int limite);

	int getCountUserChannel() const;
	void addCountUserChannel();
	void removeCountUserChannel();

	std::string getNameTopic() const;
	void setNameTopic(const std::string &topic);

	std::string getModesAsString() const;
};

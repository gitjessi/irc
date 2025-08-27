#include "../../include/Channel.hpp"

//-----------------------------------------constructeur et destructeur--------------------------------

Channel::Channel() : _channel("channel"), _name_operator(""), _key_channel(""), _inviteOnlyActif(false), _topicOperatorActif(false), _passWordActif(false),
					 _limiteUsersInChannel(0), _limiteUserIsActif(false), _countUsersChannel(0), _nameTopic("") {}

Channel::Channel(std::string &channel) : _channel(channel), _name_operator(""), _key_channel(""), _inviteOnlyActif(false), _topicOperatorActif(false), _passWordActif(false),
										 _limiteUsersInChannel(0), _limiteUserIsActif(false), _countUsersChannel(0), _nameTopic("") {}
Channel::~Channel()
{
	_clients.clear();
}

//----------------------------------------------Client----------------------------------------------------

void Channel::addClient(Client &client)
{
	if (hasClient(client.getNickName()) == false)
	{
		this->_clients.push_back(&client);

		client.sendReply(':' + client.getNickName() + '!' + client.getUserName() + '@' + client.getHostName() + " JOIN :" + _channel);
		if (_nameTopic != "")
			client.sendReply(":server 332 " + client.getNickName() + ' ' +  _channel + " :" + _nameTopic);
		else
			client.sendReply(":server 331 " + client.getNickName() + ' ' + _channel + " :No topic is set");
		_countUsersChannel++;
	}
}

bool Channel::isInChannel(const std::string& nick) const 
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i]->getNickName() == nick)
			return true;
	}
	return false;
}

void Channel::removeClient(Client &client)
{
	_clients.erase(std::find(_clients.begin(), _clients.end(), &client));
}

bool Channel::hasClient(const std::string &name_user)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it && (*it)->getNickName() == name_user)
		{
			return true;
		}
	}
	return false;
}

std::string Channel::getChannel() const { return _channel; }

//-------------------------------------------Operator------------------------------------------------------

void Channel::addOperator(const std::string &nickname)
{
	_operators.push_back(nickname);
}

bool Channel::isOperator(const std::string &name)
{
	for (std::vector<std::string>::iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		if (*it == name)
			return true;
	}
	return false;
}

void Channel::setOperator(const std::string &name)
{
	_name_operator = name;
}

std::vector<Client *> &Channel::getClient()
{
	return (_clients);
}

std::vector<std::string> Channel::getOperatorV()
{
	return (_operators);
}

std::string Channel::getOperator() const
{
	return _name_operator;
}

int Channel::removeOperator(const std::string &name)
{
	for (std::vector<std::string>::iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		if (*it == name)
		{
			_operators.erase(it);
			return 1;
		}
	}
	return 0;
}


//---------------------list all users in the channel : operator and others---------------------------------

std::string Channel::getUserList()
{
	std::string list_clients;
	for (unsigned int i = 0; i < this->_clients.size(); i++)
	{
		if (i > 0)
			list_clients += " ";
		if (isOperator(_clients[i]->getNickName()))
			list_clients += "@" + _clients[i]->getNickName();
		else
			list_clients += this->_clients[i]->getNickName();
	}
	return (list_clients);
}

std::vector<Client *> &Channel::getUserListV(void) { return (_clients); }

//----------------------------------Message all users in the channel except the user himself---------------------

void Channel::broadcast(const std::string &msg, Client &client)
{
	for (unsigned int i = 0; i < this->_clients.size(); i++)
	{
		if (_clients[i]->getNickName() == client.getNickName())
			continue;

		_clients[i]->sendReply(msg);
	}
}

//---------------------------------------Invitation to the channel --------------------------------------------------------

bool Channel::inviteOnlyIsActive() const
{
	return _inviteOnlyActif;
}

void Channel::setInviteOnly(bool value)
{
	_inviteOnlyActif = value;
}

void Channel::addInvite(const std::string &name_invite)
{
	_inviteUserInChannel.insert(name_invite);
}

bool Channel::userIsListeInvite(const std::string &name)
{
	for (std::set<std::string>::const_iterator it = _inviteUserInChannel.begin(); it != _inviteUserInChannel.end(); ++it)
	{
		if (*it == name)
			return true;
		std::cout << *it << std::endl;
	}
	return false;
}

bool Channel::isInvited(const std::string &nick) const 
{
	for (std::set<std::string>::const_iterator it = _inviteUserInChannel.begin(); it != _inviteUserInChannel.end(); ++it)
	{
		if (*it == nick)
			return true;
	}
	return false;
}

//-----------------------------------Allow changing the channel topic---------------------------------------------------------------

bool Channel::TopicOperatorIsActive() const
{
	return _topicOperatorActif;
}

void Channel::setTopicOperator(bool value)
{
	_topicOperatorActif = value;
}

std::string Channel::getNameTopic() const
{
	return _nameTopic;
}

void Channel::setNameTopic(const std::string &topic)
{
	_nameTopic = topic;
}

//----------------------------------------Key channel----------------------------------------------------------------------------

std::string Channel::getKey() const
{
	return _key_channel;
}

void Channel::setKey(const std::string &key)
{
	_key_channel = key;
}

void Channel::setPassWord(bool value)
{
	_passWordActif = value;
}

bool Channel::isPassorWord() const
{
	return _passWordActif;
}

//-------------------------------Limite users in the channel-------------------------------------------------------------------------

void Channel::setLimiteUserIsActive(bool value)
{
	_limiteUserIsActif = value;
}

bool Channel::isLimiteUserIsActive() const
{
	return _limiteUserIsActif;
}

int Channel::getLimiteUserChannel() const
{
	return _limiteUsersInChannel;
}

void Channel::setLimiteUserChannel(const int limite)
{
	_limiteUsersInChannel = limite;
}

int Channel::getCountUserChannel() const
{
	return _countUsersChannel;
}

void Channel::addCountUserChannel()
{
	_countUsersChannel += 1;
}

void Channel::removeCountUserChannel()
{
	_countUsersChannel -= 1;
}

//------------------------------mode Actif------------------------------------------

std::string Channel::getModesAsString() const
{
	std::string modes = "+";

	if (_inviteOnlyActif)
		modes += "i";

	if (_topicOperatorActif)
		modes += "t";

	if (_passWordActif)
		modes += "k";

	if (_limiteUserIsActif)
		modes += "l";

	return modes;
}
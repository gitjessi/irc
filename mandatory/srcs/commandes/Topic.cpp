#include "../../include/IRC.hpp"

bool goToTopic(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels)
{
	if (parts.size() < 2)
	{
		client.sendReply(ERR_NEEDMOREPARAMS(parts[0]));
		return false;
	}
	std::string name_channel = parts[1];
	Channel *channel_target = NULL;

	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i]->getChannel() == name_channel)
		{
			channel_target = channels[i];
			break;
		}
	}
	if (!channel_target)
	{
		client.sendReply(ERR_NOSUCHCHANNEL(name_channel));
		return false;
	}
	if (!channel_target->hasClient(client.getNickName()))
	{
		client.sendReply(ERR_NOTONCHANNEL(client.getNickName(), name_channel));
		return false;
	}

	if (parts.size() == 2)
	{
		if (channel_target->getNameTopic().empty())
			client.sendReply(RPL_NOTOPIC(client.getNickName(), name_channel));
		else
			client.sendReply(RPL_TOPIC(client.getNickName(), name_channel, channel_target->getNameTopic()));
		return true;
	}
	if (channel_target->TopicOperatorIsActive() && !channel_target->isOperator(client.getNickName()))
	{
		client.sendReply(ERR_CHANOPRIVSNEEDED(client.getNickName(), name_channel));
		return false;
	}

	std::string name_topic = parts[2];
	if (!name_topic.empty() && name_topic[0] != ':')
	{
		client.sendReply(ERR_UNKNOWNMODE(name_topic));
		return false;
	}
	name_topic = name_topic.substr(1);

	for (size_t i = 3; i < parts.size(); i++)
		name_topic += " " + parts[i];

	channel_target->setNameTopic(name_topic);
	client.sendReply(":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + "TOPIC " + name_channel +  ": " + name_topic);
	channel_target->broadcast(":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " TOPIC " + name_channel + " :" + name_topic, client);
	print_channel(client, channel_target);

	return true;
}
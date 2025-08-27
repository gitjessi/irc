#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <cstring>
#include "Channel.hpp"
#include "Client.hpp"

//-------------------command clients-------------------------

int executeCommand(std::string &line, Client &client, std::string password, std::vector<Channel *> &channels, std::vector<Client *> &clients);

//-------------------Enregistred clients----------------------------------

bool goToPass(std::string &password, std::vector<std::string> &parts, Client &client);
bool goToNickName(std::vector<std::string> &parts, Client &client, std::vector<Client *> &clients);
bool goToUser(std::vector<std::string> &parts, Client &client);

//-------------------Channel clients----------------------------------------

bool goToJoin(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients);
bool goToMode(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients);
bool goToPrivMsg(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients);
bool goToKick(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients);
bool goToInvite(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients);
bool goToTopic(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels);
void print_channel(Client &client, Channel *channel);

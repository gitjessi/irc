#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <cstring>
#include "Channel_bonus.hpp"
#include "Client_bonus.hpp"
#include "Bot_bonus.hpp"

class Bot;

//-------------------command clients-------------------------

int executeCommand(std::string &line, Client &client, std::string password, std::vector<Channel *> &channels, std::vector<Client *> &clients, Bot &bot);

//-------------------Enregistred clients----------------------------------

bool goToPass(std::string &password, std::vector<std::string> &parts, Client &client);
bool goToNickName(std::vector<std::string> &parts, Client &client, std::vector<Client *> &clients);
bool goToUser(std::vector<std::string> &parts, Client &client);

//-------------------Channel clients----------------------------------------

bool goToJoin(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients, Bot &bot);
bool goToMode(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients);
bool goToPrivMsg(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients);
bool goToKick(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients);
bool goToInvite(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels, std::vector<Client *> &clients);
bool goToTopic(std::vector<std::string> parts, Client &client, std::vector<Channel *> &channels);
void print_channel(Client &client, Channel *channel);

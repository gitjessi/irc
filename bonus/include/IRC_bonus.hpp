#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <vector>
#include <sys/epoll.h>

#include "Commande_bonus.hpp"
#include "Channel_bonus.hpp"
#include "Error_bonus.hpp"
#include "Client_bonus.hpp"
#include "Bot_bonus.hpp"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

extern int g_password;
// PARSING

int parse_port(char *av);

// UTILS

std::vector<std::string> split_buffer(const std::string& buffer);
std::vector<std::string> split(const std::string &str, char delim);

std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &v);

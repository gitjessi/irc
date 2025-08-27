#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "IRC_bonus.hpp"

class Bot : public Client
{
	public:
		Bot();
		~Bot();
		std::string myMessage(const std::string &msg);
};
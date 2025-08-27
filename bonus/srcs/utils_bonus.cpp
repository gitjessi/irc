#include "../include/IRC_bonus.hpp"
#include "../include/Client_bonus.hpp"
#include "../include/Commande_bonus.hpp"
#include "../include/Channel_bonus.hpp"

int	parse_port(char *av)
{
	int port = std::atoi(av);
	if (port <= 1024 || port >= 49151)
		port = -1;
	if (port == -1)
	{
		std::cerr << "PORT MUST BE BETWEEN 1024 and 49151" << std::endl;
		exit(1);
	}
	return (port);
}

std::vector<std::string> split_buffer(const std::string& buffer)
{
	static std::string accumulated;
	std::vector<std::string> commands;

	std::string normalized;
	for (size_t i = 0; i < buffer.size(); ++i)
	{
		if (buffer[i] == '\n')
		{
			if (i == 0 || buffer[i-1] != '\r')
				normalized += "\r\n";
			else
				normalized += '\n';
		}
		else
		{
			normalized += buffer[i];
		}
	}

	accumulated += normalized;

	size_t pos;
	while ((pos = accumulated.find("\r\n")) != std::string::npos)
	{
		std::string line = accumulated.substr(0, pos);
		accumulated.erase(0, pos + 2); // retirer le "\r\n"
		if (!line.empty())
			commands.push_back(line);
	}

	return commands;
}

std::vector<std::string> split(const std::string &str, char delim)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream stream(str);
	while (getline(stream, token, delim))
		tokens.push_back(token);
	return tokens;
}

#include "../include/Client_bonus.hpp"
#include "../include/Commande_bonus.hpp"
#include "../include/Channel_bonus.hpp"
#include "../include/IRC_bonus.hpp"
#include "../include/Server_bonus.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
		return (std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);
	else
	{
		Server server(av[2], av[1]);
		parse_port(av[1]);
		server.boot();
		server.run();
	}
}

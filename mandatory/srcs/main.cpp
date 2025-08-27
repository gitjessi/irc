#include "../include/Client.hpp"
#include "../include/Commande.hpp"
#include "../include/Channel.hpp"
#include "../include/IRC.hpp"
#include "../include/Server.hpp"

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
		std::cout << "Shutting down the server..." << std::endl;
	}
}

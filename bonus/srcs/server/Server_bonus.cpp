#include "../../include/IRC_bonus.hpp"
#include "../../include/Client_bonus.hpp"
#include "../../include/Server_bonus.hpp"
#include "../../include/Commande_bonus.hpp"
#include "../../include/Channel_bonus.hpp"

bool is_running = true;

void signalHundler(int signum)
{
	(void)signum;
	std::cout << std::endl;
	is_running = false;
}

// CONSTRUCT/DESTRUCT
Server::Server(std::string password, std::string port)
{
	this->_password = password;
	this->_servPort = std::atoi(port.c_str());
}

Server::~Server()
{
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		delete *it;
	clients.clear();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		delete *it;
	channels.clear();
}

// SERVER RUNNER
void Server::boot()
{
	sockaddr_in sst;
	std::memset(&sst.sin_zero, 0, sizeof(sst.sin_zero));
	sst.sin_family = AF_INET;
	sst.sin_addr.s_addr = htonl(INADDR_ANY);
	sst.sin_port = htons(static_cast<uint16_t>(_servPort));

	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd < 0)
	{
		std::cerr << "Error when trying to create a new socket " << std::endl;
		exit(1);
	}
	int set_socket = 1;
	setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &set_socket, sizeof set_socket);
	if (bind(_socketFd, (sockaddr *)&sst, sizeof(sst)) < 0)
	{
		close(_socketFd);
		std::cerr << "Error when trying to bind the socket " << std::endl;
		exit(1);
	}
	if (listen(_socketFd, SOMAXCONN) < 0)
	{
		close(_socketFd);
		std::cerr << "Error when trying to listen the socket " << std::endl;
		exit(1);
	}
	return;
}

void deleteChan(std::vector<Channel*>& channels, Client& client)
{
	for (size_t i = 0; i < channels.size();)
	{
		Channel* chan = channels[i];

		if (!chan->hasClient(client.getNickName()))
		{
			i++;
			continue;
		}
		chan->removeClient(client);
		if (!chan->isOperator(client.getNickName()))
		{
			i++;
			continue;
		}
		chan->removeOperator(client.getNickName());
		if (chan->getOperatorV().size() == 0 && chan->getCountUserChannel() > 1 && chan->getClient()[0]->getNickName() == "Bot")
		{
			Client *tmp = chan->getClient().begin()[1];
			chan->addOperator(tmp->getNickName());
		}
		else if (chan->getOperatorV().size() == 0)
		{
			Client *tmp = *chan->getClient().begin();
			chan->addOperator(tmp->getNickName());
		}
		if (chan->getCountUserChannel() == 0 || (chan->getCountUserChannel() == 1 && chan->getClient()[0]->getNickName() == "Bot"))
		{
			delete chan;
			channels.erase(channels.begin() + i);
			continue;
		}
		++i;
	}
}


void Server::run()
{
	signal(SIGINT, signalHundler);
	while (is_running)
	{
		Bot *roBot = new Bot();
		int i = 0;
		if (i < 1)
		{
			clients.push_back(roBot);
			i++;
		}
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(_socketFd, &readfds);
		int max_fd = _socketFd;
		for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			FD_SET((*it)->getFd(), &readfds);
			if ((*it)->getFd() > max_fd)
				max_fd = (*it)->getFd();
		}
		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if (activity < 0)
		{
			std::cout << "Shutting down the server..." << std::endl;
			break;
		}
		if (FD_ISSET(_socketFd, &readfds))
		{
			sockaddr_in client_addr;
			socklen_t addrlen = sizeof(client_addr);
			int client_fd = accept(_socketFd, reinterpret_cast<sockaddr *>(&client_addr), &addrlen);
			std::cout << client_fd << std::endl;
			Client *cli = new Client(client_fd);
			if (client_fd >= 0)
			{
				clients.push_back(cli);
				std::cout << "New client connected: fd " << client_fd << std::endl;
			}
		}
		for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end();)
		{
			int client_fd = (*it)->getFd();
			if (FD_ISSET(client_fd, &readfds))
			{
				char buffer[1024];
				ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
				if (bytes <= 0)
				{
					deleteChan(channels, **it);
					close(client_fd);
					delete *it;
					it = clients.erase(it);
					std::cout << "Client disconnected: fd " << client_fd << std::endl;
					continue;
				}
				buffer[bytes] = '\0';
				std::vector<std::string> commands = split_buffer(buffer);
				for (size_t i = 0; i < commands.size(); ++i)
				{
					if (executeCommand(commands[i], **it, _password, channels, clients, _bot) == 2)
						continue ;
				}
			}
			++it;
		}
	}
}

// GET
int Server::getSocketFd() { return (_socketFd); }
int Server::getServPort() { return (_servPort); }
std::string Server::getPassword() { return (_password); }
std::string Server::getServName() { return (_servName); }
// SET
void Server::setSocketFd(int fd) { _socketFd = fd; }
void Server::setServPort(int port) { _servPort = port; }
void Server::setPassword(std::string password) { _password = password; }
void Server::setServName(std::string serverName) { _servName = serverName; }

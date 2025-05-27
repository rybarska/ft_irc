/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:59:06 by arybarsk          #+#    #+#             */
/*   Updated: 2025/05/16 20:59:08 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//TODO: initialise all necessary server state for user/channel management
//TODO: gracefully disconnect all clients before shutdown
//TODO: deallocate any added memory / resources

Server::Server(int port, const std::string &password)
: _sockfd(-1), _port(port), _password(password), _servinfo(NULL), _cmdControl()
{
	if (!configAddrInfo())
		throw std::runtime_error("Could not set addrinfo");
}

Server::~Server()
{
	if (_servinfo)
		freeaddrinfo(_servinfo);
	if (_sockfd != -1)
	{
		close(_sockfd);
		std::cout << "Socket closed" << std::endl;
	}
	std::cout << "Server shutting down" << std::endl;
}

bool Server::configAddrInfo()
{
	std::ostringstream portStream;
	portStream << _port;
	std::string portString = portStream.str();
	
	memset(&_hints, 0, sizeof _hints); // to make sure the struct is empty
	//ai stands for addrinfo
	_hints.ai_family = AF_UNSPEC; // AF_UNSPEC - to be able to use either IPv4 or IPv6
	_hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM - TCP stream socket, not SOCK_DGRAM
	_hints.ai_flags = AI_PASSIVE; // AI_PASSIVE - to fill in our IP
	
	_status = getaddrinfo(NULL, portString.c_str(), &_hints, &_servinfo);
	if (_status != 0)
	{
		std::cerr << "Error (getaddrinfo): " << gai_strerror(_status) << std::endl;
		return false;
	}
	return true;
}

bool Server::setListeningSocket()
{
	struct addrinfo *ptr;
	
	for (ptr = _servinfo; ptr != NULL; ptr = ptr->ai_next)
	{
		_sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (_sockfd == -1)
		{
			std::cerr << "Error (socket): " << std::strerror(errno) << std::endl;
			continue ;
		}
		
		// Lose the “Address already in use” message by allowing the program to reuse the port
		int yes = 1;
		if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
		{
			close(_sockfd);
			std::cout << "Error (setsockopt)" << std::endl;
			return false;
		}
		
		if (bind(_sockfd, ptr->ai_addr, ptr->ai_addrlen) == -1)
		{
			close(_sockfd);
			std::cout << "Error (bind)" << std::endl;
			continue ;
		}
		
		break ;
	}
	
	if (ptr == NULL)
	{
		std::cout << "Could not bind socket" << std::endl;
		return false;
	}
	
	if (listen(_sockfd, BACKLOG) == -1)
	{
		close(_sockfd);
		std::cout << "Error (listen)" << std::endl;
		return false;
	}
	
	std::cout << "Server is listening on port " << _port << std::endl;
	return true;
}

bool Server::pushNewClient()
{
	struct sockaddr_storage clientAddr;
	socklen_t addrSize = sizeof clientAddr;
	
	int clientfd = accept(_sockfd, (struct sockaddr *)&clientAddr, &addrSize);
	if (clientfd == -1)
	{
		std::cerr << "Error (accept): " << std::strerror(errno) << std::endl;
		return false;
	}
	
	std::cout << "New connection established at fd " << clientfd << std::endl;
	
	struct pollfd newClient;
	newClient.fd = clientfd;
	newClient.events = POLLIN;
	newClient.revents = 0;
	_pollfds.push_back(newClient);
	
	_clients.insert(std::make_pair<int, Client *>(clientfd, new Client(clientfd)));
	
	//TODO: Consider prompting for password
	
	return true;
}

bool Server::getLineFromRingBuffer(Client *client, std::string &line)
{
	const size_t MAX_LEN = 512; // per RFC 2812, includes \r\n
	
	char c;
	line.clear();
	size_t len = 0;
	bool overflow = false;
	
	while (client->getRingBuffer().getElem(c))
	{
		if (c == '\n')
			break ;
		if (len >= MAX_LEN - 2) // 2 for \r\n
			overflow = true;
		if (!overflow && c != '\r')
		{
			line += c;
			len++;
		}
	}
	if (overflow)
	{
		std::cerr << "Error: line too long" << std::endl;
		line.clear();
		return false;
	}
	return (!line.empty());
}

bool Server::processClientInput(size_t index)
{
//TODO: sanitize input for protocol compliance, e.g. max 512 bytes per RFC 2812
//TODO: implement rate-limiting or anti-flood protection if necessary
//TODO: handle errors for _cmdControl.processCommand more gracefully
	
	int clientfd = _pollfds[index].fd;
	char tempBuffer[512];
	int bufLen = sizeof(tempBuffer) - 1;
	int flags = 0;
	
	// readout comes in bytes
	ssize_t readout = recv(clientfd, tempBuffer, bufLen, flags);
	if (readout <= 0)
	{
		if (readout == 0)
		{
			std::cout << "Connection at fd " << clientfd << " closed." << std::endl;
		}
		else
		{
			std::cerr << "Error (recv): " << std::strerror(errno) << std::endl;
		}
		close (clientfd);
		_pollfds.erase(_pollfds.begin() + index);
		delete _clients[clientfd];
		_clients.erase(clientfd);
		return false;
	}
	
	Client* client = _clients[clientfd];
	
	for (ssize_t i= 0; i < readout; i++)
	{
		client->addInputToRingBuffer(tempBuffer[i]);
	}
	
	std::string line;
	Message msg;
	while (getLineFromRingBuffer(client, line))
	{
		//std::cout << "Message received from client " << clientfd << ": " << line << std::endl;
		
		msg.parseMessage(line);
		
		//std::cout << "Parsed Message: " << std::endl;
		//std::cout << "prefix: " << msg.getPrefix() << std::endl;
		//std::cout << "command: " << msg.getCommand() << std::endl;
		//for (size_t i=0; i < msg.getParams().size(); i++)
		//	std::cout << "param " << i << " : " << msg.getParams()[i] << std::endl;
		//std::cout << "trailing: " << msg.getTrailing() << std::endl;
		
		_cmdControl.processCommand(*client, msg);
		
		attemptAuth(client);
		
		attemptRegistration(client);
	}
	
	return true;
}

void Server::pollEvents()
{
//TODO: use timeout or explicitly detect and clean up disconnected or idle clients
//TODO: Add signal handling for graceful shutdown, e.g. SIGINT

	struct pollfd pfd;
	pfd.fd = _sockfd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollfds.push_back(pfd);
	
	while (true)
	{
		if (_pollfds.empty())
			continue ;
		
		int eventCount = poll(_pollfds.data(), _pollfds.size(), -1);
		
		if (eventCount == -1)
		{
			std::cerr << "Error (poll): " << std::strerror(errno) << std::endl;
			break ;
		}
		
		// iterate through connections
		for (size_t i=0; i < _pollfds.size(); i++)
		{
			// check if a connection is ready to read
			if (_pollfds[i].revents & (POLLIN | POLLHUP))
			{
				if (_pollfds[i].fd == _sockfd)
				{
					if (!pushNewClient())
						continue ;
				}
				else
				{
					if (processClientInput(i))
						continue ;
				}
			}
		}
	}
}

bool Server::attemptAuth(Client *client)
{
//TODO: Send proper IRC ERR_PASSWDMISMATCH (464) if incorrect password
//TODO: Possibly disconnect clients afer failed auth attempts
//TODO: Add limit to number of auth attempts

	if (client->isRegistered())
		return false;
	
	if (!client->_hasPass)
	{
		std::cout << "Cannot authenticate yet, missing password" << std::endl;
		return false;
	}
	
	if (client->getPassword() != _password)
	{
		std::cerr << "Error (wrong password)" << std::endl;
		// TODO: send error message
		return false;
	}
	
	client->setAuthed();
	
	//TODO consider sending ERR_PASSWDMISMATCH(464) here if password fails
	
	return true;
}

bool Server::attemptRegistration(Client *client)
{
	if (client->isRegistered())
		return false;
	
	if (!client->isAuthed() || !client->_hasNick || !client->_hasUser)
	{
		std::cout << "Cannot register yet, missing info" << std::endl;
		return false;
	}
	
	client->setRegistered();
	
	if (client->isRegistered())
		std::cout << "Registered!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		
	//TODO send proper IRC welcome message (RPL_WELCOME 001) and other numeric replies (002-004)
	
	return true;
}

bool Server::getGoing()
{
	if (!setListeningSocket())
		throw std::runtime_error("Could not set socket");
	
	pollEvents();
	
	return true;
}


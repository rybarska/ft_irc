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

Server::Server(int port, const std::string &password)
: _sockfd(-1), _port(port), _password(password), _ipAddress("127.0.0.1"), _servinfo(NULL)
{
	if (!isPortValid(port))
		throw std::invalid_argument("Invalid port: number must be between 1024 and 65535");
	if (!isPswdValid(password))
		throw std::invalid_argument("Invalid password: length must be between 6 and 64");
	if (!configAddrInfo(_ipAddress))
		throw std::runtime_error("Could not set addrinfo");
};

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
};

bool Server::isPortValid(int port)
{
	return port >= 1024 && port <= 65535;
};

bool Server::isPswdValid(const std::string &pswd)
{
	return pswd.length() >= 6 && pswd.length() <= 64;
};

bool Server::configAddrInfo(std::string &_ipAddress)
{
	std::ostringstream portStream;
	portStream << _port;
	std::string portString = portStream.str();
	
	memset(&_hints, 0, sizeof _hints); // to make sure the struct is empty
	//ai stands for addrinfo
	_hints.ai_family = AF_UNSPEC; // AF_UNSPEC - to be able to use either IPv4 or IPv6
	_hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM - TCP stream socket, not SOCK_DGRAM
	_hints.ai_flags = AI_PASSIVE; // AI_PASSIVE - to fill in our IP
	
	_status = getaddrinfo(_ipAddress.c_str(), portString.c_str(), &_hints, &_servinfo);
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

void Server::setNewConnection()
{
	struct sockaddr_storage clientAddr;
	socklen_t addrSize = sizeof clientAddr;
	
	int clientfd = accept(_sockfd, (struct sockaddr *)&clientAddr, &addrSize);
	if (clientfd == -1)
	{
		std::cerr << "Error (accept): " << std::strerror(errno) << std::endl;
		return ;
	}
	
	std::cout << "New client connection accepted at fd " << clientfd << std::endl;
	
	struct pollfd newClient;
	newClient.fd = clientfd;
	newClient.events = POLLIN;
	_pollfds.push_back(newClient);
}

bool Server::getGoing()
{
	if (!setListeningSocket())
		throw std::runtime_error("Could not set socket");
	
	struct pollfd pfd;
	pfd.fd = _sockfd;
	pfd.events = POLLIN;
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
					setNewConnection();
				}
				/*else
				{
					handleClientMessage(); //TODO
				}*/
			}
		}
	}
	
	return true;
}


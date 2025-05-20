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
: _sockfd(-1), _port(port), _password(password), _servinfo(NULL)
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

bool Server::acceptNewClient()
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
	
	return true;
}

bool Server::processClientInput(size_t index)
{
	int clientfd = _pollfds[index].fd;
	char buffer[1024];
	int bufLen = sizeof(buffer) - 1;
	int flags = 0;
	
	// readout comes in bytes
	ssize_t readout = recv(clientfd, buffer, bufLen, flags);
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
		return false;
	}
	else
	{
		buffer[readout] = '\0';
		std::cout << "Message from fd " << clientfd << ": " << buffer << std::endl;
		return true;
	}
}

void Server::pollEvents()
{	
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
		//int eventCount = poll(_pollfds.data(), 1, -1);
		
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
					if (!acceptNewClient())
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

bool Server::getGoing()
{
	if (!setListeningSocket())
		throw std::runtime_error("Could not set socket");
	
	pollEvents();
	
	return true;
}


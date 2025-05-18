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
: _sockfd(-1), _port(port), _password(password), _host("127.0.0.1"), _servinfo(NULL)
{
	if (!isPortValid(port))
		throw std::invalid_argument("Invalid port: number must be between 1024 and 65535");
	if (!isPswdValid(password))
		throw std::invalid_argument("Invalid password: length must be between 6 and 64");
	if (!setAddrInfo(_host))
		throw std::runtime_error("Could not set addrinfo");
};

Server::~Server()
{
	if (_servinfo)
		freeaddrinfo(_servinfo);
	if (_sockfd != -1)
	{
		close(_sockfd);
		std::cout << "Server socket closed" << std::endl;
	}
};

bool Server::isPortValid(int port)
{
	return port >= 1024 && port <= 65535;
};

bool Server::isPswdValid(std::string pswd)
{
	return pswd.length() >= 6 && pswd.length() <= 64;
};

bool Server::setAddrInfo(std::string &_host)
{
	std::ostringstream portStream;
	portStream << _port;
	std::string portString = portStream.str();
	
	memset(&_hints, 0, sizeof _hints);
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_socktype = SOCK_STREAM;
	_status = getaddrinfo(_host.c_str(), portString.c_str(), &_hints, &_servinfo);
	if (_status != 0)
	{
		std::cerr << "Error (getaddrinfo): " << gai_strerror(_status) << std::endl;
		return false;
	}
	return true;
}

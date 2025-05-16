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

Server::Server(int port, const std::string & password)
: _port(port), _password(password)
{
	if (!isPortValid(port))
		throw std::invalid_argument("Invalid port: number must be between 1024 and 65535");
	if (!isPswdValid(password))
		throw std::invalid_argument("Invalid password: length must be between 6 and 64");
};

Server::~Server()
{};

bool Server::isPortValid(int port)
{
	return port >= 1024 && port <= 65535;
};

bool Server::isPswdValid(std::string pswd)
{
	return pswd.length() >= 6 && pswd.length() <= 64;
};

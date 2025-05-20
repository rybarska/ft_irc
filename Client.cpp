/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:14:18 by arybarsk          #+#    #+#             */
/*   Updated: 2025/05/20 15:14:21 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, const std::string &password)
: _clientfd(fd), _password(password), _nickname(""), _username(""), _buffer("")
{}

Client::~Client()
{}

int Client::getFd() const
{
	return _clientfd;
}

std::string const &Client::getPassword() const
{
	return _password;
}

std::string const &Client::getNickname() const
{
	return _nickname;
}

std::string const &Client::getUsername() const
{
	return _username;
}

std::string const &Client::getBuffer() const
{
	return _buffer;
}

void Client::setNickname(const std::string &nickname)
{
	_nickname = nickname;
}

void Client::setUsername(const std::string &username)
{
	_username = username;
}

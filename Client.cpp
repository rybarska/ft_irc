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

Client::Client(int fd)
: _clientfd(fd), _clientPassword(""), _nickname(""), _username(""), _registered(false)
{}

Client::~Client()
{}

int Client::getFd() const
{
	return _clientfd;
}

std::string const &Client::getPassword() const
{
	return _clientPassword;
}

std::string const &Client::getNickname() const
{
	return _nickname;
}

std::string const &Client::getUsername() const
{
	return _username;
}

void Client::setNickname(const std::string &nickname)
{
	_nickname = nickname;
}

void Client::setUsername(const std::string &username)
{
	_username = username;
}

void Client::addInputToRingBuffer(char c)
{
	_ringbuffer.addElem(c);
}

void Client::clearRingBuffer()
{
	_ringbuffer.clearBuffer();
}

bool Client::isAuthed()
{
	if (!_registered || _nickname.empty() || _username.empty())
		return false;
	return true;
}

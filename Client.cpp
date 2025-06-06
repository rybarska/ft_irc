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
: _hasPass(false), _hasNick(false), _hasUser(false),
_clientfd(fd), _clientPassword(""), _nickname(""), _username(""),
_authed(false), _registered(false)
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

RingBuffer<char, 512> &Client::getRingBuffer()
{
	return _ringbuffer;
}

void Client::setPassword(const std::string &password)
{
	_clientPassword = password;
}

void Client::setNickname(const std::string &nickname)
{
	_nickname = nickname;
}

void Client::setUsername(const std::string &username)
{
	_username = username;
}

void Client::setAuthed()
{
	_authed = true;
}

void Client::setRegistered()
{
	_registered = true;
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
	if (!_authed)
		return false;
	return true;
}

bool Client::isRegistered()
{
	if (!_registered)
		return false;
	return true;
}

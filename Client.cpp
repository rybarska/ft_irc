/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:14:18 by arybarsk          #+#    #+#             */
/*   Updated: 2025/06/14 21:19:43 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <vector>

Client::Client(int fd)
: _hasPass(false), _hasNick(false), _hasUser(false),
_clientfd(fd), _clientPassword(""), _nickname(""), _username(""),
_authed(false), _registered(false), _disconnected(false)
{
	std::cout << "Client created at fd " << fd << std::endl;
}

Client::~Client()
{
	std::cout << "Client destroyed at fd " << _clientfd << std::endl;
}

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

void Client::setDisconnected()
{
	_disconnected = true;
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

bool Client::isDisconnected()
{
	if (!_disconnected)
		return false;
	return true;
}

bool Client::sendMsgToClient(std::string const &msg)
{
	if (_clientfd == -1)
		return false;
	std::string formattedMsg = msg + "\r\n";
	if (send(_clientfd, formattedMsg.c_str(), formattedMsg.size(), 0) == -1)
	{
		// EPIPE is graceful close
		// ECONNRESET is e.g. crashing, rebooting, or improper close() call
		if (errno == EPIPE || errno == ECONNRESET)
		{
			std::cerr << "Client at fd " << _clientfd << "disconnected" << std::endl;
			std::cerr << "(send error: " << std::strerror(errno) << ")" << std::endl;
			setDisconnected();
		}
		else
		{
			std::cerr << "send error: " << std::strerror(errno) << std::endl;
		}
		return false;
	}
	return true;
}

void Client::sendMessage(const std::string &msg)
{
    if (send(_clientfd, msg.c_str(), msg.size(), 0) == -1)
    {
        std::cerr << "Error sending message to fd " << _clientfd 
                  << ": " << std::strerror(errno) << std::endl;
    }
}


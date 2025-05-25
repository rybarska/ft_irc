/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:23:00 by arybarsk          #+#    #+#             */
/*   Updated: 2025/05/23 18:23:04 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message(){}
		
Message::~Message(){}

std::string const &Message::getPrefix() const
{
	return _prefix;
}

std::string const &Message::getCommand() const
{
	return _command;
}

std::vector<std::string> const &Message::getParams() const
{
	return _params;
}

std::string const &Message::getTrailing() const
{
	return _trailing;
}

void Message::parseMessage(std::string const &msg)
{
	_prefix.clear();
	_command.clear();
	_params.clear();
	_trailing.clear();
	
	size_t ending = msg.find_last_not_of("\n\r");
	if (ending == std::string::npos)
		return ;
	
	const std::string line = msg.substr(0, ending + 1);
	
	size_t pos = 0;
	size_t next;
	
	if (line[pos] == ':')
	{
		next = line.find(' ', pos);
		if (next == std::string::npos)
			return ;
		_prefix = line.substr(pos + 1, next - pos - 1);
		pos = next + 1;
	}
	
	next = line.find(' ');
	_command = line.substr(pos, next - pos);
	if (_command.empty())
		return ;
	
	if (next == std::string::npos)
		pos = line.size();
	else
		pos = next + 1;
	
	while (pos < line.size())
	{
		if (line[pos] == ':')
		{
			_trailing = line.substr(pos + 1);
			break ;
		}
		next = line.find(' ', pos);	
		_params.push_back(line.substr(pos, next - pos));
		if (next == std::string::npos)
			break ;
		pos = next + 1;
	}
	
}

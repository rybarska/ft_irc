/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:23:00 by arybarsk          #+#    #+#             */
/*   Updated: 2025/06/14 20:04:12 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include <sstream>
#include <string>

Message::Message() : _raw(""), _prefix(""), _command(""), _trailing("")
{
}

Message::~Message()
{
}

const std::string &Message::getPrefix() const
{
    return _prefix;
}

const std::string &Message::getCommand() const
{
    return _command;
}

const std::vector<std::string> &Message::getParams() const
{
    return _params;
}

const std::string &Message::getTrailing() const
{
    return _trailing;
}

const std::string &Message::getRawMessage() const
{
    return _raw;
}

void Message::parseMessage(std::string const &msg)
{
    _raw = msg; // store the entire raw input

    std::istringstream iss(msg);
    // if the message starts with ':' then the first token is the prefix
    if (!msg.empty() && msg[0] == ':')
    {
        iss >> _prefix;
        // remove the leading ':'
        if (!_prefix.empty())
            _prefix = _prefix.substr(1);
    }
    iss >> _command;
    
    _params.clear();
    _trailing.clear();
    std::string token;
    while (iss >> token)
    {
        if (token[0] == ':')
        {
            // Everything after ':' is the trailing text
            _trailing = token.substr(1);
            std::string rest;
            getline(iss, rest);
            _trailing += rest;
            break;
        }
        else
        {
            _params.push_back(token);
        }
    }
}

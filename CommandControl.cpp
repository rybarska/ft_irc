/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandControl.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:14:44 by arybarsk          #+#    #+#             */
/*   Updated: 2025/05/25 16:14:45 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandControl.hpp"
#include "Client.hpp"

//TODO: implement full IRC registration flow, including NICK and USER validation.
//TODO: implement command routing for JOIN, PRIVMSG, PART, QUIT, MODE, TOPIC etc.

CommandControl::CommandControl()
{
	_controls["PASS"] = &CommandControl::processPass;
	_controls["NICK"] = &CommandControl::processNick;
	_controls["USER"] = &CommandControl::processUser;
	_controls["JOIN"] = &CommandControl::processJoin;
	_controls["PRIVMSG"] = &CommandControl::processPriv;
}
		
CommandControl::~CommandControl(){}

void CommandControl::processCommand(Client &client, const Message &msg)
{
	std::map<std::string, ControlPFn>::iterator it = _controls.find(msg.getCommand());
	if (it != _controls.end())
	{
		ControlPFn foo = it->second;
		(this->*foo)(client, msg);
	}
	else
		processUnknown(client, msg);
}

void CommandControl::processPass(Client &client, const Message &msg)
{
	if (client.isAuthed())
	{
		// TODO: send error to client
		return ;
	}
	if (!msg.getParams().empty())
		client._hasPass = true;
	//else
	//{
		// TODO: send error to client
	//}
	
	client.setPassword(msg.getParams()[0]);
	
	client._hasPass = true;
}

void CommandControl::processNick(Client &client, const Message &msg)
{
	std::cout << "NICK: ";
	if (msg.getParams().empty())
		std::cout << "(none)" << std::endl;
	else
	{
		std::cout << msg.getParams()[0] << std::endl;
		client.setNickname(msg.getParams()[0]);
		client._hasNick = true;
	}
}

void CommandControl::processUser(Client &client, const Message &msg)
{
	std::cout << "USER: ";
	if (msg.getParams().empty())
		std::cout << "(none)" << std::endl;
	else
	{
		std::cout << msg.getParams()[0] << std::endl;
		client.setUsername(msg.getParams()[0]);
		client._hasUser = true;
	}
}

void CommandControl::processJoin(Client &client, const Message &msg)
{
	(void) client; //will need it later
	std::cout << "JOIN: ";
	if (msg.getParams().empty())
		std::cout << "(none)" << std::endl;
	else
	{
		std::cout << msg.getParams()[0] << std::endl;
		// TODO join the channel
	}
}

void CommandControl::processPriv(Client &client, const Message &msg)
{
	(void) client; //will need it later
	std::cout << "PRIV: ";
	if (msg.getParams().empty())
		std::cout << "(none)" << std::endl;
	else
	{
		std::cout << msg.getParams()[0] << std::endl;
		// TODO send private message
	}
}

void CommandControl::processUnknown(Client &client, const Message &msg)
{
	(void) client; //might need it later
	std::cout << "Unknown command: " << msg.getCommand() << std::endl;
	// TODO see if there is anything else to do here
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandControl.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:14:44 by arybarsk          #+#    #+#             */
/*   Updated: 2025/06/14 20:45:55 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandControl.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

extern Server* g_serverInstance;

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
    if (msg.getParams().empty())
    {
        std::cout << "JOIN: Missing channel name" << std::endl;
        return;
    }
    
    const std::string channelName = msg.getParams()[0];
    
    // Use the server instance to get or create the channel
    if (g_serverInstance)
    {
        Channel *channel = g_serverInstance->getOrCreateChannel(channelName);
        channel->addClient(&client);
        std::cout << "Client " << client.getFd() << " joined channel " << channelName << std::endl;
    }
    else
    {
        std::cout << "Error: server instance not available" << std::endl;
    }
}

void CommandControl::processPriv(Client &client, const Message &msg)
{
    // Check that both a target and trailing message exist
    if (msg.getParams().empty() || msg.getTrailing().empty())
    {
        std::cout << "PRIVMSG: Missing target or message text" << std::endl;
        return;
    }
    
    // For this project assume target is always a channel name
    std::string target = msg.getParams()[0];
    std::string messageText = msg.getTrailing();
    
    // Retrieve the channel (you can use getOrCreateChannel if needed,
    // though in a real IRC a missing channel is an error for PRIVMSG)
    Channel *channel = g_serverInstance->getOrCreateChannel(target);
    
    // Check if client is in that channel; if not, you may want to send an error.
    if (!channel->hasClient(&client))
    {
        std::cout << "Client " << client.getFd() 
                  << " is not in channel " << target << std::endl;
        return;
    }
    
    // Format the message in IRC PRIVMSG style:
    // :<NICK> PRIVMSG <CHANNEL> :<message>
    std::string outMsg = ":" + client.getNickname() + " PRIVMSG " + target 
                         + " :" + messageText + "\r\n";
    
    // Broadcast the message to all clients in the channel except the sender
    channel->broadcastMessage(outMsg, &client);
    
    std::cout << "Processed PRIVMSG from client " << client.getFd() << std::endl;
}

void CommandControl::processUnknown(Client &client, const Message &msg)
{
    (void)client;
    std::cout << "Unknown command: " << msg.getCommand() << std::endl;
    
    // Instead of calling processGeneralMessage, simply notify about the unknown command.
    // You can also send an error message back to the client here if desired.
}

// void CommandControl::processGeneralMessage(Client &client, const Message &msg)
// {
//     std::vector<Channel*> channels = g_serverInstance->getChannelsForClient(&client);
//     if (channels.empty())
//     {
//         std::cout << "Client " << client.getFd() 
//                   << " is not in any channel." << std::endl;
//         return;
//     }
    
//     // Use the full raw message
//     std::string messageContent = msg.getRawMessage();
    
//     // For each channel, format the message as: NICK &<CHANNEL>: msg
//     for (size_t i = 0; i < channels.size(); i++)
//     {
//         std::string outMsg = client.getNickname() + " &" + channels[i]->getName() 
//                              + ": " + messageContent + "\r\n";
//         channels[i]->broadcastMessage(outMsg, &client);
//     }
    
//     std::cout << "Broadcasted general message from client " 
//               << client.getFd() << std::endl;
// }
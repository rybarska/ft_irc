/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandControl.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:14:44 by arybarsk          #+#    #+#             */
/*   Updated: 2025/07/05 18:11:52 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandControl.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "IRCReplies.hpp"
#include <cstdlib> // for atoi

extern Server* g_serverInstance;

CommandControl::CommandControl()
{
	_controls["PASS"] = &CommandControl::processPass;
	_controls["NICK"] = &CommandControl::processNick;
	_controls["USER"] = &CommandControl::processUser;
	_controls["JOIN"] = &CommandControl::processJoin;
	_controls["PRIVMSG"] = &CommandControl::processPriv;
    _controls["CAP"] = &CommandControl::processCap;
    _controls["PING"] = &CommandControl::processPing;
    _controls["MODE"] = &CommandControl::processMode;
    _controls["QUIT"] = &CommandControl::processQuit;
    _controls["TOPIC"] = &CommandControl::processTopic;
    _controls["KICK"] = &CommandControl::processKick;
    _controls["INVITE"] = &CommandControl::processInvite;
    _controls["PART"] = &CommandControl::processPart;
    
}
		
CommandControl::~CommandControl(){}

void CommandControl::processTopic(Client &client, const Message &msg)
{
    if (!client.isRegistered()) {
        IRCReplies::sendNotRegistered(&client);
        return;
    }
    
    if (msg.getParams().empty()) {
        IRCReplies::sendNeedMoreParams(&client, "TOPIC");
        return;
    }
    
    const std::string channelName = msg.getParams()[0];
    
    if (g_serverInstance) {
        Channel *channel = g_serverInstance->getChannel(channelName);
        
        if (!channel) {
            IRCReplies::sendNoSuchChannel(&client, channelName);
            return;
        }
        
        if (!channel->hasClient(&client)) {
            IRCReplies::sendNotOnChannel(&client, channelName);
            return;
        }
        
        // If no topic parameter provided, show current topic
        if (msg.getParams().size() == 1 && msg.getTrailing().empty()) {
            if (channel->hasTopic()) {
                IRCReplies::sendTopic(&client, channelName, channel->getTopic());
            } else {
                IRCReplies::sendNoTopic(&client, channelName);
            }
            return;
        }
        
        // Setting topic - check if topic protection is enabled
        if (channel->isTopicProtected() && !channel->isOperator(&client)) {
            IRCReplies::sendChanOPrivsNeeded(&client, channelName);
            return;
        }
        
        std::string newTopic = msg.getTrailing();
        channel->setTopic(newTopic);
        
        // Broadcast topic change to all users in channel
        std::string topicMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 TOPIC " + channelName + " :" + newTopic + "\r\n";
        channel->broadcastMessage(topicMsg, NULL); // Send to ALL users including sender
        
        std::cout << "Topic set for " << channelName << ": " << newTopic << std::endl;
    }
}


void CommandControl::processQuit(Client &client, const Message &msg)
{
    std::string quitMsg = "Quit";
    if (!msg.getTrailing().empty())
        quitMsg = msg.getTrailing();
    
    // Notify all channels the user was in
    std::vector<Channel*> channels = g_serverInstance->getChannelsForClient(&client);
    std::cout << "DEBUG: Client " << client.getFd() << " is in " << channels.size() << " channels" << std::endl;
    
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        std::cout << "DEBUG: Broadcasting quit to channel: " << (*it)->getName() << std::endl;
        // FIX: Use proper IRC QUIT format
        std::string hostmask = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 QUIT :" + quitMsg + "\r\n";
        (*it)->broadcastMessage(hostmask, &client);
        (*it)->removeClient(&client);
    }
    
    std::cout << "Client " << client.getFd() << " quit: " << quitMsg << std::endl;
}

void CommandControl::processMode(Client &client, const Message &msg)
{
    if (!client.isRegistered()) {
        IRCReplies::sendNotRegistered(&client);
        return;
    }
    
    if (msg.getParams().empty()) {
        IRCReplies::sendNeedMoreParams(&client, "MODE");
        return;
    }
    
    const std::string target = msg.getParams()[0];
    
    // Check if it's a channel mode request
    if (target[0] == '#' || target[0] == '&') {
        processChannelMode(client, msg);
    } else {
        // User mode - not implemented yet
        std::cout << "User MODE not implemented yet" << std::endl;
    }
}

void CommandControl::processChannelMode(Client &client, const Message &msg)
{
    const std::string channelName = msg.getParams()[0];
    
    if (!g_serverInstance) {
        std::cout << "ERROR: Server instance not available" << std::endl;
        return;
    }
    
    Channel *channel = g_serverInstance->getChannel(channelName);
    if (!channel) {
        IRCReplies::sendNoSuchChannel(&client, channelName);
        return;
    }
    
    // Check if client is on the channel
    if (!channel->hasClient(&client)) {
        IRCReplies::sendNotOnChannel(&client, channelName);
        return;
    }
    
    // If no mode string provided, return current modes
    if (msg.getParams().size() == 1) {
        std::string modeString = channel->getModeString();
        if (modeString.empty()) {
            modeString = "+";
        }
        
        // Send channel mode reply (324)
        std::string reply = ":" + std::string("ircserv") + " 324 " + client.getNickname() + " " + channelName + " " + modeString;
        client.sendMsgToClient(reply);
        return;
    }
    
    // Parse mode changes
    std::string modeString = msg.getParams()[1];
    std::vector<std::string> modeParams;
    
    // Collect additional parameters for modes that need them
    for (size_t i = 2; i < msg.getParams().size(); ++i) {
        modeParams.push_back(msg.getParams()[i]);
    }
    
    // Process mode changes
    processModeChanges(client, channel, modeString, modeParams);
}

void CommandControl::processPing(Client &client, const Message &msg)
{
    if (!msg.getParams().empty())
    {
        client.sendMsgToClient("PONG ircserv :" + msg.getParams()[0] + "\r\n");
        std::cout << "PING/PONG handled" << std::endl;
    }
}

void CommandControl::processCap(Client &client, const Message &msg)
{   
    (void)msg;
    //so Irssi doesn't disconnect
  
    client.sendMsgToClient("CAP * LS :\r\n");
    std::cout << "CAP command handled" << std::endl;
}

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
        IRCReplies::sendAlreadyRegistered(&client);
        return ;
    }
    if (!msg.getParams().empty())
    {
        client.setPassword(msg.getParams()[0]);
        client._hasPass = true;
        std::cout << "PASS command received: " << msg.getParams()[0] << std::endl;
    }
    else
    {
        IRCReplies::sendNeedMoreParams(&client, "PASS");
        std::cout << "PASS command missing parameter!" << std::endl;
    }
}

void CommandControl::processNick(Client &client, const Message &msg)
{
	std::cout << "NICK: ";
	if (msg.getParams().empty())
	{
		std::cout << "(none)" << std::endl;
		IRCReplies::sendNeedMoreParams(&client, "NICK");
		return;
	}
	
	const std::string &newNick = msg.getParams()[0];
	std::cout << newNick << std::endl;
	
	// Check if nickname is already in use (if server instance is available)
	if (g_serverInstance) {
		Client *existingClient = g_serverInstance->getClientByNickname(newNick);
		if (existingClient && existingClient != &client) {
			IRCReplies::sendNicknameInUse(&client, newNick);
			return;
		}
	}
	
	client.setNickname(newNick);
	client._hasNick = true;
}

void CommandControl::processUser(Client &client, const Message &msg)
{
	std::cout << "USER: ";
	// USER command needs: username [hostname] [servername] :realname
	// Require at least username + realname (trailing)
	if (msg.getParams().size() < 1 || msg.getTrailing().empty())
	{
		std::cout << "(need at least username and realname)" << std::endl;
		IRCReplies::sendNeedMoreParams(&client, "USER");
		return;
	}
	
	std::cout << msg.getParams()[0] << std::endl;
	client.setUsername(msg.getParams()[0]);
	client._hasUser = true;
}

void CommandControl::processJoin(Client &client, const Message &msg)
{
    if (!client.isRegistered()) {
        IRCReplies::sendNotRegistered(&client);
        return;
    }
    
    if (msg.getParams().empty()) {
        IRCReplies::sendNeedMoreParams(&client, "JOIN");
        return;
    }
    
    const std::string channelName = msg.getParams()[0];
    
    // Validate channel name format - must start with #
    if (channelName.empty() || channelName[0] != '#') {
        IRCReplies::sendNoSuchChannel(&client, channelName);
        return;
    }
    
    std::string channelKey;
    
    // Check if a key was provided
    if (msg.getParams().size() > 1) {
        channelKey = msg.getParams()[1];
    }
    
    if (g_serverInstance) {
        Channel *channel = g_serverInstance->getOrCreateChannel(channelName);
        
        // Check if already in channel
        if (channel->hasClient(&client)) {
            return; // Already in channel, ignore
        }
        
        // Check channel modes before allowing join
        
        // Check if channel is invite-only
        if (channel->isInviteOnly()) {
            // Check if the client is invited
            if (!channel->isInvited(&client)) {
                IRCReplies::sendInviteOnlyChan(&client, channelName);
                return;
            }
            // Remove from invite list once they join
            channel->removeFromInviteList(&client);
        }
        
        // Check if channel has a key and if the provided key matches
        if (channel->hasKey()) {
            if (channelKey.empty() || channelKey != channel->getKey()) {
                IRCReplies::sendBadChannelKey(&client, channelName);
                return;
            }
        }
        
        // Check user limit
        if (channel->hasUserLimit()) {
            if ((int)channel->getClients().size() >= channel->getUserLimit()) {
                IRCReplies::sendChannelIsFull(&client, channelName);
                return;
            }
        }
        
        channel->addClient(&client);
        
        // Send JOIN confirmation to joining client
        std::string joinMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 JOIN " + channelName + "\r\n";
        client.sendMsgToClient(joinMsg);
        
        // Send topic (if exists) or no topic
        if (channel->hasTopic()) {
            IRCReplies::sendTopic(&client, channelName, channel->getTopic());
        } else {
            IRCReplies::sendNoTopic(&client, channelName);
        }
        
        // Send names list (353) and end of names (366)
        std::vector<Client*> clients = channel->getClients();
        std::string names;
        for (size_t i = 0; i < clients.size(); i++) {
            if (i > 0) names += " ";
            // Add @ for operators, nothing for regular users
            if (channel->isOperator(clients[i])) {
                names += "@";
            }
            names += clients[i]->getNickname();
        }
        IRCReplies::sendNameReply(&client, channelName, names);
        IRCReplies::sendEndOfNames(&client, channelName);
        
        // Notify other users in channel
        std::string notifyMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 JOIN " + channelName + "\r\n";
        channel->broadcastMessage(notifyMsg, &client);
        
        std::cout << "Client " << client.getFd() << " joined channel " << channelName << std::endl;
    }
}

void CommandControl::processPriv(Client &client, const Message &msg)
{
    if (!client.isRegistered()) {
        IRCReplies::sendNotRegistered(&client);
        return;
    }
    
    // Check that both a target and trailing message exist
    if (msg.getParams().empty() || msg.getTrailing().empty())
    {
        IRCReplies::sendNeedMoreParams(&client, "PRIVMSG");
        std::cout << "PRIVMSG: Missing target or message text" << std::endl;
        return;
    }
    
    std::string target = msg.getParams()[0];
    std::string messageText = msg.getTrailing();
    
    if (!g_serverInstance) {
        std::cout << "ERROR: Server instance not available" << std::endl;
        return;
    }
    
    // Check if target is a channel (starts with #)
    if (target[0] == '#') {
        Channel *channel = g_serverInstance->getChannel(target);
        
        if (!channel) {
            IRCReplies::sendNoSuchChannel(&client, target);
            return;
        }
        
        // Check if client is in that channel
        if (!channel->hasClient(&client))
        {
            IRCReplies::sendCannotSendToChan(&client, target);
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
    }
    else {
        // Private message to user
        Client *targetClient = g_serverInstance->getClientByNickname(target);
        if (!targetClient) {
            IRCReplies::sendNoSuchNick(&client, target);
            return;
        }
        
        // Send private message
        std::string outMsg = ":" + client.getNickname() + " PRIVMSG " + target 
                             + " :" + messageText + "\r\n";
        targetClient->sendMsgToClient(outMsg);
    }
    
    std::cout << "Processed PRIVMSG from client " << client.getFd() << std::endl;
}

void CommandControl::processUnknown(Client &client, const Message &msg)
{
    if (client.isRegistered()) {
        // Send ERR_UNKNOWNCOMMAND (421) for registered clients
        std::string reply = ":ircserv 421 " + client.getNickname() + " " + msg.getCommand() + " :Unknown command\r\n";
        client.sendMsgToClient(reply);
    }
    std::cout << "Unknown command: " << msg.getCommand() << std::endl;
}

void CommandControl::processKick(Client &client, const Message &msg)
{
    if (!client.isRegistered()) {
        IRCReplies::sendNotRegistered(&client);
        return;
    }
    
    if (msg.getParams().size() < 2) {
        IRCReplies::sendNeedMoreParams(&client, "KICK");
        return;
    }
    
    const std::string channelName = msg.getParams()[0];
    const std::string targetNick = msg.getParams()[1];
    std::string reason = "No reason given";
    
    if (!msg.getTrailing().empty()) {
        reason = msg.getTrailing();
    }
    
    if (!g_serverInstance) {
        std::cout << "ERROR: Server instance not available" << std::endl;
        return;
    }
    
    // Find the channel
    Channel *channel = g_serverInstance->getChannel(channelName);
    if (!channel) {
        IRCReplies::sendNoSuchChannel(&client, channelName);
        return;
    }
    
    // Check if the client is on the channel
    if (!channel->hasClient(&client)) {
        IRCReplies::sendNotOnChannel(&client, channelName);
        return;
    }
    
    // Check if the client is a channel operator
    if (!channel->isOperator(&client)) {
        IRCReplies::sendChanOPrivsNeeded(&client, channelName);
        return;
    }
    
    // Find the target client
    Client *targetClient = g_serverInstance->getClientByNickname(targetNick);
    if (!targetClient) {
        IRCReplies::sendNoSuchNick(&client, targetNick);
        return;
    }
    
    // Check if the target client is on the channel
    if (!channel->hasClient(targetClient)) {
        IRCReplies::sendNotOnChannel(&client, targetNick + " " + channelName);
        return;
    }
    
    // Broadcast the KICK message to all channel members
    std::string kickMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 KICK " + channelName + " " + targetNick + " :" + reason;
    channel->broadcastMessage(kickMsg, NULL); // NULL sender means send to everyone including the kicker
    
    // Remove the target client from the channel
    channel->removeClient(targetClient);
    
    // If the target was an operator, remove operator status too
    if (channel->isOperator(targetClient)) {
        channel->removeOperator(targetClient);
    }
    
    std::cout << "KICK: " << client.getNickname() << " kicked " << targetNick << " from " << channelName << " (" << reason << ")" << std::endl;
}

void CommandControl::processInvite(Client &client, const Message &msg)
{
    if (!client.isRegistered()) {
        IRCReplies::sendNotRegistered(&client);
        return;
    }
    
    if (msg.getParams().size() < 2) {
        IRCReplies::sendNeedMoreParams(&client, "INVITE");
        return;
    }
    
    const std::string targetNick = msg.getParams()[0];
    const std::string channelName = msg.getParams()[1];
    
    if (!g_serverInstance) {
        std::cout << "ERROR: Server instance not available" << std::endl;
        return;
    }
    
    // Find the target client
    Client *targetClient = g_serverInstance->getClientByNickname(targetNick);
    if (!targetClient) {
        IRCReplies::sendNoSuchNick(&client, targetNick);
        return;
    }
    
    // Find the channel (it must exist for invites)
    Channel *channel = g_serverInstance->getChannel(channelName);
    if (!channel) {
        IRCReplies::sendNoSuchChannel(&client, channelName);
        return;
    }
    
    // Check if the inviting client is on the channel
    if (!channel->hasClient(&client)) {
        IRCReplies::sendNotOnChannel(&client, channelName);
        return;
    }
    
    // Check if the target client is already on the channel
    if (channel->hasClient(targetClient)) {
        IRCReplies::sendUserOnChannel(&client, targetNick, channelName);
        return;
    }
    
    // For invite-only channels, only operators can invite
    if (channel->isInviteOnly() && !channel->isOperator(&client)) {
        IRCReplies::sendChanOPrivsNeeded(&client, channelName);
        return;
    }
    
    // Add target client to invite list
    channel->addToInviteList(targetClient);
    
    // Send invitation confirmation to the inviting client
    IRCReplies::sendInviting(&client, targetNick, channelName);
    
    // Send invitation to the target client
    std::string inviteMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 INVITE " + targetNick + " :" + channelName;
    targetClient->sendMsgToClient(inviteMsg);
    
    std::cout << "INVITE: " << client.getNickname() << " invited " << targetNick << " to " << channelName << std::endl;
}

void CommandControl::processModeChanges(Client &client, Channel *channel, const std::string &modeString, const std::vector<std::string> &params)
{
    // Check if client is channel operator (required for most mode changes)
    bool isOp = channel->isOperator(&client);
    
    bool adding = true; // Start with adding modes
    size_t paramIndex = 0;
    std::string appliedModes;
    std::string appliedParams;
    
    for (size_t i = 0; i < modeString.length(); ++i) {
        char mode = modeString[i];
        
        if (mode == '+') {
            adding = true;
            continue;
        } else if (mode == '-') {
            adding = false;
            continue;
        }
        
        // Handle individual modes
        switch (mode) {
            case 'i': // Invite-only
                if (!isOp) {
                    IRCReplies::sendChanOPrivsNeeded(&client, channel->getName());
                    continue;
                }
                channel->setInviteOnly(adding);
                appliedModes += (adding ? "+" : "-");
                appliedModes += "i";
                break;
                
            case 't': // Topic protection
                if (!isOp) {
                    IRCReplies::sendChanOPrivsNeeded(&client, channel->getName());
                    continue;
                }
                channel->setTopicProtected(adding);
                appliedModes += (adding ? "+" : "-");
                appliedModes += "t";
                break;
                
            case 'k': // Channel key
                if (!isOp) {
                    IRCReplies::sendChanOPrivsNeeded(&client, channel->getName());
                    continue;
                }
                if (adding) {
                    if (paramIndex < params.size()) {
                        channel->setKey(params[paramIndex]);
                        appliedModes += "+k";
                        if (!appliedParams.empty()) appliedParams += " ";
                        appliedParams += params[paramIndex];
                        paramIndex++;
                    }
                } else {
                    channel->removeKey();
                    appliedModes += "-k";
                }
                break;
                
            case 'l': // User limit
                if (!isOp) {
                    IRCReplies::sendChanOPrivsNeeded(&client, channel->getName());
                    continue;
                }
                if (adding) {
                    if (paramIndex < params.size()) {
                        int limit = atoi(params[paramIndex].c_str());
                        if (limit > 0) {
                            channel->setUserLimit(limit);
                            appliedModes += "+l";
                            if (!appliedParams.empty()) appliedParams += " ";
                            appliedParams += params[paramIndex];
                        }
                        paramIndex++;
                    }
                } else {
                    channel->removeUserLimit();
                    appliedModes += "-l";
                }
                break;
                
            case 'o': // Operator privileges
                if (!isOp) {
                    IRCReplies::sendChanOPrivsNeeded(&client, channel->getName());
                    continue;
                }
                if (paramIndex < params.size()) {
                    Client *targetClient = g_serverInstance->getClientByNickname(params[paramIndex]);
                    if (targetClient && channel->hasClient(targetClient)) {
                        if (adding) {
                            channel->addOperator(targetClient);
                            appliedModes += "+o";
                        } else {
                            channel->removeOperator(targetClient);
                            appliedModes += "-o";
                        }
                        if (!appliedParams.empty()) appliedParams += " ";
                        appliedParams += params[paramIndex];
                    }
                    paramIndex++;
                }
                break;
                
            default:
                IRCReplies::sendUnknownMode(&client, std::string(1, mode));
                break;
        }
    }
    
    // Broadcast mode changes to all channel members if any modes were applied
    if (!appliedModes.empty()) {
        std::string modeMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 MODE " + channel->getName() + " " + appliedModes;
        if (!appliedParams.empty()) {
            modeMsg += " " + appliedParams;
        }
        channel->broadcastMessage(modeMsg, NULL); // Send to everyone including the mode setter
        
        std::cout << "MODE: " << client.getNickname() << " set modes " << appliedModes << " on " << channel->getName() << std::endl;
    }
}

void CommandControl::processPart(Client &client, const Message &msg)
{
    if (!client.isRegistered()) {
        IRCReplies::sendNotRegistered(&client);
        return;
    }
    
    if (msg.getParams().empty()) {
        IRCReplies::sendNeedMoreParams(&client, "PART");
        return;
    }
    
    // Get the channel name(s) - PART can handle multiple channels separated by commas
    std::string channelList = msg.getParams()[0];
    std::string partMessage = "Leaving";
    
    // Check if a part message was provided
    if (!msg.getTrailing().empty()) {
        partMessage = msg.getTrailing();
    } else if (msg.getParams().size() > 1) {
        // Part message can also be in the second parameter
        partMessage = msg.getParams()[1];
    }
    
    if (!g_serverInstance) {
        std::cout << "ERROR: Server instance not available" << std::endl;
        return;
    }
    
    // Parse channel list - split by commas for multiple channels
    std::vector<std::string> channels;
    std::string current;
    for (size_t i = 0; i < channelList.length(); ++i) {
        if (channelList[i] == ',') {
            if (!current.empty()) {
                channels.push_back(current);
                current.clear();
            }
        } else {
            current += channelList[i];
        }
    }
    if (!current.empty()) {
        channels.push_back(current);
    }
    
    // Process each channel
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
        const std::string &channelName = *it;
        
        // Find the channel
        Channel *channel = g_serverInstance->getChannel(channelName);
        if (!channel) {
            IRCReplies::sendNoSuchChannel(&client, channelName);
            continue;
        }
        
        // Check if the client is on the channel
        if (!channel->hasClient(&client)) {
            IRCReplies::sendNotOnChannel(&client, channelName);
            continue;
        }
        
        // Broadcast PART message to all channel members (including the leaving user)
        std::string partMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1 PART " + channelName + " :" + partMessage;
        channel->broadcastMessage(partMsg, NULL); // Send to everyone including the person leaving
        
        // Remove the client from the channel
        channel->removeClient(&client);
        
        // If the client was an operator, remove operator status
        if (channel->isOperator(&client)) {
            channel->removeOperator(&client);
        }
        
        // Remove from invite list if they were invited (cleanup)
        channel->removeFromInviteList(&client);
        
        std::cout << "PART: " << client.getNickname() << " left " << channelName << " (" << partMessage << ")" << std::endl;
    }
}
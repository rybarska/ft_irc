/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandControl.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:14:35 by arybarsk          #+#    #+#             */
/*   Updated: 2025/06/28 15:46:37 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDCONTROL_HPP
#define COMMANDCONTROL_HPP

#include <iostream>
#include <map>
#include "Message.hpp"

class Client;
class Channel;

class CommandControl
{
	public:
		CommandControl();
		
		~CommandControl();
		
		void processCommand(Client &client, const Message &msg);
	private:
		typedef void (CommandControl::*ControlPFn) (Client &, const Message &);
		std::map<std::string, ControlPFn> _controls;
		
		void processPass(Client &client, const Message &msg);
		void processNick(Client &client, const Message &msg);
		void processUser(Client &client, const Message &msg);
		void processJoin(Client &client, const Message &msg);
		void processPriv(Client &client, const Message &msg);
		void processUnknown(Client &client, const Message &msg);
		void processGeneralMessage(Client &client, const Message &msg);
		void processCap(Client &client, const Message &msg);
		void processPing(Client &client, const Message &msg);
		void processMode(Client &client, const Message &msg);
		void processQuit(Client &client, const Message &msg);
		void processTopic(Client &client, const Message &msg);
		void processKick(Client &client, const Message &msg);
		void processInvite(Client &client, const Message &msg);
		void processPart(Client &client, const Message &msg);
		
		// Helper methods for MODE command
		void processChannelMode(Client &client, const Message &msg);
		void processModeChanges(Client &client, Channel *channel, const std::string &modeString, const std::vector<std::string> &params);
		
		CommandControl(const CommandControl &source);
		CommandControl & operator = (const CommandControl &source);
};

#endif

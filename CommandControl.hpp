/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandControl.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:14:35 by arybarsk          #+#    #+#             */
/*   Updated: 2025/05/25 16:14:39 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDCONTROL_HPP
#define COMMANDCONTROL_HPP

#include <iostream>
#include <map>
#include "Message.hpp"

class Client;

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
		
		CommandControl(const CommandControl &source);
		CommandControl & operator = (const CommandControl &source);
};

#endif

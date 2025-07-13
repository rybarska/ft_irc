/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:40:24 by arybarsk          #+#    #+#             */
/*   Updated: 2025/06/29 15:40:25 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <sys/types.h>
#include <netdb.h> // for struct addrinfo, getaddrinfo()
#include <cstring> //for memset()
#include <unistd.h>
#include <sstream>
#include <sys/socket.h>
#include <poll.h>
#include <errno.h>

extern volatile sig_atomic_t g_oOnBotting;

class Bot
{
	public:
		Bot(const std::string &host, int port, const std::string &password);

		~Bot();

		bool getRunning();
		void cleanup();
	private:
		int _botfd;
		std::string _host;
		int _port;
		std::string _password;
		int _status;
		struct addrinfo _hints; // will point to struct addrinfo that we fill out
		struct addrinfo *_servinfo; // will point to the results

		bool configBotAddrInfo();
		bool setSocketAndConnect();
		bool sendMsgToServer(std::string const &msg);
		bool registerBot();

		Bot();
		Bot(const Bot &source);
		Bot & operator = (const Bot &source);
};

#endif

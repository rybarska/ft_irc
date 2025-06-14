/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:59:19 by arybarsk          #+#    #+#             */
/*   Updated: 2025/06/14 19:32:45 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <errno.h>
#include <stdexcept> // for runtime_error
#include <unistd.h> // for close()
#include <netdb.h> // for struct addrinfo, getaddrinfo()
#include <netinet/in.h>
#include <cstring> //for memset()
#include <sstream>
#include <sys/socket.h>
#include <poll.h>
#include <vector>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Client.hpp"
#include "Message.hpp"
#include "CommandControl.hpp"
#include "Channel.hpp"

#define BACKLOG 10 // number of pending connections that the queue will hold

extern volatile __sig_atomic_t g_oOn;

class Server
{
	public:
		Server(int port, const std::string &password);
		
		~Server();
		
		bool getGoing();
		
		void cleanup();
		Channel* getOrCreateChannel(const std::string &name);
		std::vector<Channel*> getChannelsForClient(Client *client);
	private:
		int _sockfd;
		int _port;
		std::string _password;
		int _status;
		struct addrinfo _hints; // will point to struct addrinfo that we fill out
		struct addrinfo *_servinfo; // will point to the results
		
		std::vector<struct pollfd> _pollfds;
		
		std::map<int, Client*> _clients;
		std::map<std::string, Channel*> _channels;
		
		bool configAddrInfo();
		
		bool setListeningSocket();
		void pollEvents();
		bool pushNewClient();
		bool processClientInput(size_t index);
		
		bool getLineFromRingBuffer(Client *client, std::string &line);
		
		CommandControl _cmdControl;
		
		bool attemptAuth(Client *client);
		bool attemptRegistration(Client *client);
		
		Server();
		Server(const Server &source);
		Server & operator = (const Server &source);
};

#endif

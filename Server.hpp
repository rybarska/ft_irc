/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:59:19 by arybarsk          #+#    #+#             */
/*   Updated: 2025/05/16 20:59:22 by arybarsk         ###   ########.fr       */
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
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 10 // number of pending connections that the queue will hold

class Server
{
	public:
		Server(int port, const std::string &password);
		
		~Server();
		
		bool getGoing();
	private:
		int _sockfd;
		int _port;
		std::string _password;
		int _status;
		struct addrinfo _hints; // will point to struct addrinfo that we fill out
		struct addrinfo *_servinfo; // will point to the results
		
		std::vector<struct pollfd> _pollfds;
		
		bool configAddrInfo();
		
		bool setListeningSocket();
		void pollEvents();
		bool acceptNewClient();
		bool processClientInput(size_t index);
		
		Server();
		Server(const Server &source);
		Server & operator = (const Server &source);
};

#endif

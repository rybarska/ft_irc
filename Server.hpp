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
#include <unistd.h> // for close()
#include <netdb.h> // for struct addrinfo, getaddrinfo()
#include <cstring> //for memset()
#include <sstream>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>

#define BACKLOG 10 // number of pending connections that the queue will hold

class Server
{
	public:
		Server(int port, const std::string &password);
		
		~Server();
		
		bool startListening();
	private:
		int _sockfd;
		int _port;
		std::string _password;
		std::string _ipAddress;
		int _status;
		struct addrinfo _hints; // will point to struct addrinfo that we fill out
		struct addrinfo *_servinfo; // will point to the results
		
		std::vector<struct pollfd> _pollfds;
		
		bool isPortValid(int port);
		bool isPswdValid(const std::string &pswd);
		
		bool setAddrInfo(std::string &_ipAddress);
		
		bool setListeningSocket();
		
		Server();
		Server(const Server &source);
		Server & operator = (const Server &source);
};

#endif

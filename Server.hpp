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
#include <unistd.h> // for close()
#include <netdb.h> // for struct addrinfo, getaddrinfo()
#include <cstring> //for memset()
#include <sstream>

class Server
{
	public:
		Server(int port, const std::string &password);
		
		~Server();
		
	private:
		int _sockfd;
		int _port;
		std::string _password;
		std::string _host;
		int _status;
		struct addrinfo _hints;
		struct addrinfo *_servinfo;
		
		bool isPortValid(int port);
		bool isPswdValid(std::string pswd);
		
		bool setAddrInfo(std::string &host);
		
		Server();
		Server(const Server &source);
		Server & operator = (const Server &source);
};

#endif

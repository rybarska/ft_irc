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

class Server
{
	public:
		Server(int port, const std::string & password);
		
		~Server();
		
	private:
		int _port;
		std::string _password;
		
		bool isPortValid(int port);
		bool isPswdValid(std::string pswd);
		
		Server();
		Server(const Server &source);
		Server & operator = (const Server &source);
};

#endif

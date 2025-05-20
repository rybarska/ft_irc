/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:14:09 by arybarsk          #+#    #+#             */
/*   Updated: 2025/05/20 15:14:13 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
	public:
		Client(int fd, const std::string &password);
		
		~Client();
		
		int getFd() const;
		std::string const &getPassword() const;
		std::string const &getNickname() const;
		std::string const &getUsername() const;
		std::string const &getBuffer() const;
		
		void setNickname(const std::string &nickname);
		void setUsername(const std::string &username);
		
	private:
		int _clientfd;
		std::string _password;
		std::string _nickname;
		std::string _username;
		std::string _buffer;
		
		Client();
		Client(const Client &source);
		Client & operator = (const Client &source);
};

#endif

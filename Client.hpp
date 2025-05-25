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
#include "RingBuffer.hpp"

class Client
{
	public:
		Client(int fd);
		
		~Client();
		
		int getFd() const;
		std::string const &getPassword() const;
		std::string const &getNickname() const;
		std::string const &getUsername() const;
		RingBuffer<char, 1024> &getRingBuffer();
		
		void setNickname(const std::string &nickname);
		void setUsername(const std::string &username);
		
		void addInputToRingBuffer(char c);
		void clearRingBuffer();
		
		bool isAuthed();
		
	private:
		int _clientfd;
		std::string _clientPassword;
		std::string _nickname;
		std::string _username;
		bool _hasPass;
		bool _hasNick;
		bool _hasUser;
		bool _registered;
		
		RingBuffer<char, 1024> _ringbuffer;
		
		Client();
		Client(const Client &source);
		Client & operator = (const Client &source);
};

#endif

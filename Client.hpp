/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:14:09 by arybarsk          #+#    #+#             */
/*   Updated: 2025/06/14 19:12:54 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
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
		RingBuffer<char, 512> &getRingBuffer();
		
		void setPassword(const std::string &password);
		void setNickname(const std::string &nickname);
		void setUsername(const std::string &username);
		void setAuthed();
		void setRegistered();
		
		void addInputToRingBuffer(char c);
		void clearRingBuffer();
		
		bool isAuthed();
		bool isRegistered();
		
		bool sendMsgToClient(std::string const &msg);
		
		bool _hasPass;
		bool _hasNick;
		bool _hasUser;

		void sendMessage(const std::string &msg);
		
		
	private:
		int _clientfd;
		std::string _clientPassword;
		std::string _nickname;
		std::string _username;
		bool _authed;
		bool _registered;
		
		RingBuffer<char, 512> _ringbuffer;
		
		Client();
		Client(const Client &source);
		Client & operator = (const Client &source);
};

#endif

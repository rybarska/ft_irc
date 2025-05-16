/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:58:50 by arybarsk          #+#    #+#             */
/*   Updated: 2025/05/16 20:58:55 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdlib> // for std::atoi
#include <sstream>

bool isOnlyWhitespace(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		if (std::string(argv[i]).find_first_not_of(" \t\n\r") != std::string::npos)
			return (false);
	}
	return (true);
}

int main(int argc, char **argv)
{	
	try
	{
		if (argc != 3 || isOnlyWhitespace(argc, argv))
		{
			std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
			return (1);
		}
		
		int port = std::atoi(argv[1]);
		std::string password(argv[2]);
		
		Server server(port, password);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Server exception: " << e.what() << std::endl;
		return (1);
	}
	
	return (0);
}

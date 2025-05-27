/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandLineUtils.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:01:50 by arybarsk          #+#    #+#             */
/*   Updated: 2025/05/27 17:01:51 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib> // for std::atoi()
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

bool parseCommandLineArgs(int argc, char **argv, int &port, std::string &password)
{
	if (argc != 3 || isOnlyWhitespace(argc, argv))
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (false);
	}
	
	std::istringstream iss(argv[1]);
	iss >> port;
	if (!iss.eof() || iss.fail() || port <= 1024 || port >= 65535)
	{
		std::cerr << "Error: port must be a number between 1024 and 65535" << std::endl;
		return (false);
	}
	
	password = argv[2];
	if (!iss.eof() || iss.fail() || password.length() < 6 || password.length() > 64)
	{
		std::cerr << "Error: password length must be between 6 and 64" << std::endl;
		return (false);
	}
	
	return (true);
}

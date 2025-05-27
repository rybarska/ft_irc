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
#include "commandLineUtils.cpp"

//TODO: might want to add error or log system instead of std::cerr / std::cout

int main(int argc, char **argv)
{	
	try
	{
		int port;
		std::string password;
		
		if (!parseCommandLineArgs(argc, argv, port, password))
			return (1);
		
		Server server(port, password);
		
		server.getGoing();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Server exception: " << e.what() << std::endl;
		return (1);
	}
	
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:07:15 by arybarsk          #+#    #+#             */
/*   Updated: 2025/06/29 16:07:16 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <cstdio>
#include "Bot.hpp"

volatile sig_atomic_t g_oOnBotting = 1;

void	handle_sig(int sig)
{
	(void) sig;
	g_oOnBotting = 0;
}

int main()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sig;
	sa.sa_flags = SA_RESTART;
	
	if (sigaction(SIGINT, &sa, NULL) == -1 // for Ctrl-C
		|| sigaction(SIGTERM, &sa, NULL) == -1) // for kill command
	{
		perror("sigaction");
		return (1);
	}
	
	struct sigaction sa_pipe;
	memset(&sa_pipe, 0, sizeof(sa_pipe));
	sa_pipe.sa_handler = SIG_IGN;
	sa_pipe.sa_flags = 0;
	
	if (sigaction(SIGPIPE, &sa_pipe, NULL) == -1) 
	{
		perror("sigaction(SIGPIPE)");
		return (1);
	}
	
	Bot bot("127.0.0.1", 1069, "123456");
	
	try
	{
		if (!bot.getRunning())
		{
			std::cerr << "Bot could not get running" << std::endl;
			return 1;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception in bot's main: " << e.what() << std::endl;
		return (1);
	}
	
	return 0;
}

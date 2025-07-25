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

#include <cstdio>
#include "Server.hpp"
#include "commandLineUtils.cpp"

volatile sig_atomic_t g_oOn = 1;
volatile sig_atomic_t g_sigCaught = 0;

void	handle_sig(int sig)
{
	g_oOn = 0;
	g_sigCaught = sig;
}

// this: struct sigaction sa = {};
// would be better instead of memset (cleaner, more type-safe)
// but in C++98, this is technically not standard.

// with SA_RESTART the system automatically checks if errno == EINTR
// for every syscall like poll() interrupted by a signal

// sigemptyset() is forbidden in the subject
// otherwise, this line would allow only SIGINT during handler:
// sigemptyset(sa.sa_mask);
// here we can live without it because we memset() the whole struct to zero
// which is effectively the same as sigemptyset()

// SIGINT is Ctrl-C
// SIGTERM is in kill command:
// ps aux | grep ircserv
// kill -SIGTERM <process number>

// SIGPIPE is ignored so send/recv errors can be handled in code, not via signals

int main(int argc, char **argv)
{	
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sig;
	sa.sa_flags = SA_RESTART;
	
	if (sigaction(SIGINT, &sa, NULL) == -1 
		|| sigaction(SIGTERM, &sa, NULL) == -1)
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
	
	try
	{
		int port;
		std::string password;
		
		if (!parseCommandLineArgs(argc, argv, port, password))
			return (1);
		
		Server server(port, password);
		
		server.getGoing();
		
		if (g_sigCaught == SIGINT)
			std::cout << "Caught SIGINT, server shutting down" << std::endl;
		else if (g_sigCaught == SIGTERM)
			std::cout << "Caught SIGTERM, server shutting down" << std::endl;
		else
			std::cout << "Caught unknown signal: (" << g_sigCaught << "), server shutting down" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Server exception: " << e.what() << std::endl;
		return (1);
	}
	
	return (0);
}

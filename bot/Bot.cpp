/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:40:12 by arybarsk          #+#    #+#             */
/*   Updated: 2025/07/05 18:53:08 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(const std::string &host, int port, const std::string &password)
: _botfd(-1), _host(host), _port(port), _password(password), _servinfo(NULL)
{
    if (!configBotAddrInfo())
        throw std::runtime_error("Bot could not set addrinfo");
}

Bot::~Bot()
{
    cleanup();
}

void Bot::cleanup()
{
    if (_servinfo)
    {
        freeaddrinfo(_servinfo);
        _servinfo = NULL;
    }
    if (_botfd != -1)
    {
        close(_botfd);
        _botfd = -1;
    }
}

bool Bot::configBotAddrInfo()
{
    std::ostringstream portStream;
    portStream << _port;
    std::string portString = portStream.str();

    memset(&_hints, 0, sizeof _hints); // to make sure the struct is empty
    //ai stands for addrinfo
    _hints.ai_family = AF_UNSPEC; // AF_UNSPEC - to be able to use either IPv4 or IPv6
    _hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM - TCP stream socket, not SOCK_DGRAM
    _hints.ai_flags = AI_PASSIVE; // AI_PASSIVE - to fill in our IP

    _status = getaddrinfo(_host.c_str(), portString.c_str(), &_hints, &_servinfo);
    if (_status != 0)
    {
        std::cerr << "Error (bot getaddrinfo): " << gai_strerror(_status) << std::endl;
        return false;
    }
    return true;
}

bool Bot::setSocketAndConnect()
{
    struct addrinfo *ptr;

    for (ptr = _servinfo; ptr != NULL; ptr = ptr->ai_next)
    {
        _botfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (_botfd == -1)
        {
            std::cerr << "Error (bot socket): " << std::strerror(errno) << std::endl;
            continue ;
        }

        if (connect(_botfd, ptr->ai_addr, ptr->ai_addrlen) == -1)
        {
            close(_botfd);
            std::cout << "Error (bot connect)" << std::endl;
            continue ;
        }

        break ;
    }

    if (ptr == NULL)
    {
        std::cerr << "Bot: could not connect" << std::endl;
        return false;
    }
    return true;
}

bool Bot::sendMsgToServer(std::string const &msg)
{
   std::string formattedMsg = msg + "\r\n";
   ssize_t sentBytes = send(_botfd, formattedMsg.c_str(), formattedMsg.size(), 0);
   if (sentBytes == -1)
   {
        std::cerr << "Bot: send error: " << strerror(errno) << std::endl;
        return false;
   }
   return true;
}

bool Bot::registerBot()
{
   if (!sendMsgToServer("PASS " + _password))
      return false;
   if (!sendMsgToServer("NICK bot"))
      return false;
   if (!sendMsgToServer("USER bot 0 * :just a simple bot"))
      return false;
   return true;
}

bool Bot::getRunning()
{
   if (!setSocketAndConnect())
      throw std::runtime_error("Bot could not set socket");
   registerBot();
   sleep(1);
   sendMsgToServer("JOIN #berlin");
   sendMsgToServer("PRIVMSG #berlin :I use Arch Linux");
   while (g_oOnBotting)
   {
		if (!sendMsgToServer("PRIVMSG #berlin :Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse accumsan velit eu lacus aliquam, vitae finibus nisl imperdiet. Aenean tristique nisi sit amet faucibus iaculis. Nam orci neque, fringilla quis bibendum quis, egestas at mi. Proin a sagittis urna, vitae fermentum ex. Suspendisse congue massa tellus, vel facilisis nulla condimentum nec. Aliquam malesuada metus eget vulputate finibus."))
            break ;
        usleep(50000);
	}
   return true;
}

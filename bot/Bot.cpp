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
   if (send(_botfd, formattedMsg.c_str(), formattedMsg.size(), 0) == -1)
      return false;
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
		sendMsgToServer("PRIVMSG #berlin :Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse accumsan velit eu lacus aliquam, vitae finibus nisl imperdiet. Suspendisse efficitur tortor id risus dignissim sodales. Aenean tristique nisi sit amet faucibus iaculis. Nam orci neque, fringilla quis bibendum quis, egestas at mi. Proin a sagittis urna, vitae fermentum ex. Suspendisse congue massa tellus, vel facilisis nulla condimentum nec. Aliquam malesuada metus eget vulputate finibus. Aenean ut pharetra risus.Nullam lobortis placerat metus, eu elementum sem convallis non. Nam scelerisque erat sit amet sapien lacinia, id lobortis magna luctus. Donec porta tincidunt nisl, quis ultrices nunc tincidunt et. Praesent nec neque nec purus lacinia tincidunt sed ut ex. Donec euismod est sit amet purus gravida, eget tempor lorem semper. Integer commodo, lectus a commodo pretium, leo augue mattis ex, quis feugiat diam quam nec ex. Phasellus imperdiet lacinia mauris, in feugiat tortor venenatis eget. Donec lacinia mi magna, sit amet efficitur odio egestas nec. Suspendisse ac neque erat. Vivamus et ante turpis. Cras pretium tortor eget orci posuere, eu tincidunt lectus vestibulum.Duis rhoncus imperdiet ullamcorper. Etiam non purus magna. Cras non sapien non felis venenatis imperdiet. Proin auctor pellentesque tincidunt. Etiam vulputate magna nec nulla pharetra, ut lacinia arcu pellentesque. Nullam tincidunt, mi sed scelerisque lacinia, neque lacus convallis erat, non mattis diam lorem et lectus. Proin eu leo molestie, fringilla nulla vel, dictum mauris. Curabitur molestie faucibus felis, eu vestibulum nulla interdum eget.Suspendisse venenatis porttitor turpis, ut maximus augue pretium sit amet. In vehicula a lorem a finibus. Ut sed sollicitudin sem. Etiam vitae turpis lorem. Duis non mauris sodales, ullamcorper orci id, varius augue. Quisque pharetra risus vel odio dictum fermentum. Cras vel libero elementum, suscipit turpis eget, fringilla ipsum.Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Fusce molestie sapien at neque aliquet, vitae sodales neque rhoncus. Proin sed magna et justo sodales volutpat quis vel lorem. Sed at eleifend purus. Aenean quis lorem malesuada, ullamcorper erat eget, iaculis neque. Aliquam venenatis ornare neque, lacinia dignissim lectus elementum et. Nam vel lacus sed lectus tempus posuere. Integer tellus ligula, euismod ac quam a, scelerisque placerat ligula. Nam lobortis est tempus sem tincidunt mattis. Nulla vitae pellentesque odio. Donec ut ultrices leo, sed mollis massa. Mauris purus nibh, venenatis molestie elit a, auctor placerat tellus. Phasellus egestas suscipit lacus, placerat viverra mauris euismod at.");
	}
   //sleep(1000000000);  
   return true;
}

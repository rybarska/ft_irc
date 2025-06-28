/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaranov <ibaranov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:22:49 by arybarsk          #+#    #+#             */
/*   Updated: 2025/06/14 20:03:33 by ibaranov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <sstream>
#include <vector>

class Message
{
    public:
        Message();
        ~Message();
        
        std::string const &getPrefix() const;
        std::string const &getCommand() const;
        std::vector<std::string> const &getParams() const;
        std::string const &getTrailing() const;
        
        void parseMessage(std::string const &msg);
        const std::string &getRawMessage() const;
        
    private:
        std::string _raw; 
        std::string _prefix;
        std::string _command;
        std::vector<std::string> _params;
        std::string _trailing;
        
        Message(const Message &source);
        Message & operator = (const Message &source);
};

#endif
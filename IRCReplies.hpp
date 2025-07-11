#ifndef IRCREPLIES_HPP
#define IRCREPLIES_HPP

#include <string>

class Client;

class IRCReplies
{
public:
    // Channel replies
    static void sendNameReply(Client *client, const std::string &channel, const std::string &names);
    static void sendEndOfNames(Client *client, const std::string &channel);
    static void sendTopic(Client *client, const std::string &channel, const std::string &topic);
    static void sendNoTopic(Client *client, const std::string &channel);
    
    // WHO replies
    static void sendWhoReply(Client *client, const std::string &channel, const std::string &user, const std::string &host, const std::string &server, const std::string &nick, const std::string &flags, const std::string &realname);
    static void sendEndOfWho(Client *client, const std::string &name);
    
    // Success replies
    static void sendInviting(Client *client, const std::string &nick, const std::string &channel);
    
    // Error replies - Essential ones only
    static void sendNeedMoreParams(Client *client, const std::string &command);
    static void sendNicknameInUse(Client *client, const std::string &nick);
    static void sendNoSuchNick(Client *client, const std::string &nick);
    static void sendNoSuchChannel(Client *client, const std::string &channel);
    static void sendNotOnChannel(Client *client, const std::string &channel);
    static void sendUserOnChannel(Client *client, const std::string &nick, const std::string &channel);
    static void sendChanOPrivsNeeded(Client *client, const std::string &channel);
    static void sendCannotSendToChan(Client *client, const std::string &channel);
    static void sendPasswordMismatch(Client *client);
    static void sendAlreadyRegistered(Client *client);
    static void sendNotRegistered(Client *client);
    
    // Channel mode errors
    static void sendUnknownMode(Client *client, const std::string &mode);
    static void sendChannelIsFull(Client *client, const std::string &channel);
    static void sendInviteOnlyChan(Client *client, const std::string &channel);
    static void sendBadChannelKey(Client *client, const std::string &channel);

private:
    static void sendReply(Client *client, int code, const std::string &message);
};

#endif
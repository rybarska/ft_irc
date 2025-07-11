#include "IRCReplies.hpp"
#include "Client.hpp"
#include <sstream>

void IRCReplies::sendReply(Client *client, int code, const std::string &message)
{
    std::ostringstream oss;
    oss << ":ircserv " << code << " " << client->getNickname() << " " << message << "\r\n";
    client->sendMsgToClient(oss.str());
}

// Channel replies
void IRCReplies::sendNameReply(Client *client, const std::string &channel, const std::string &names)
{
    sendReply(client, 353, "= " + channel + " :" + names);
}

void IRCReplies::sendEndOfNames(Client *client, const std::string &channel)
{
    sendReply(client, 366, channel + " :End of /NAMES list");
}

void IRCReplies::sendTopic(Client *client, const std::string &channel, const std::string &topic)
{
    sendReply(client, 332, channel + " :" + topic);
}

void IRCReplies::sendNoTopic(Client *client, const std::string &channel)
{
    sendReply(client, 331, channel + " :No topic is set");
}

// WHO replies
void IRCReplies::sendWhoReply(Client *client, const std::string &channel, const std::string &user, const std::string &host, const std::string &server, const std::string &nick, const std::string &flags, const std::string &realname)
{
    sendReply(client, 352, channel + " " + user + " " + host + " " + server + " " + nick + " " + flags + " :0 " + realname);
}

void IRCReplies::sendEndOfWho(Client *client, const std::string &name)
{
    sendReply(client, 315, name + " :End of /WHO list");
}

// Success replies
void IRCReplies::sendInviting(Client *client, const std::string &nick, const std::string &channel)
{
    sendReply(client, 341, nick + " " + channel);
}

// Error replies - KEEP ONLY ONE COPY OF EACH
void IRCReplies::sendNeedMoreParams(Client *client, const std::string &command)
{
    sendReply(client, 461, command + " :Not enough parameters");
}

void IRCReplies::sendNicknameInUse(Client *client, const std::string &nick)
{
    sendReply(client, 433, nick + " :Nickname is already in use");
}

void IRCReplies::sendNoSuchNick(Client *client, const std::string &nick)
{
    sendReply(client, 401, nick + " :No such nick/channel");
}

void IRCReplies::sendNoSuchChannel(Client *client, const std::string &channel)
{
    sendReply(client, 403, channel + " :No such channel");
}

void IRCReplies::sendNotOnChannel(Client *client, const std::string &channel)
{
    sendReply(client, 442, channel + " :You're not on that channel");
}

void IRCReplies::sendUserOnChannel(Client *client, const std::string &nick, const std::string &channel)
{
    sendReply(client, 443, nick + " " + channel + " :is already on channel");
}

void IRCReplies::sendChanOPrivsNeeded(Client *client, const std::string &channel)
{
    sendReply(client, 482, channel + " :You're not channel operator");
}

void IRCReplies::sendCannotSendToChan(Client *client, const std::string &channel)
{
    sendReply(client, 404, channel + " :Cannot send to channel");
}

void IRCReplies::sendPasswordMismatch(Client *client)
{
    sendReply(client, 464, ":Password incorrect");
}

void IRCReplies::sendAlreadyRegistered(Client *client)
{
    sendReply(client, 462, ":You may not reregister");
}

void IRCReplies::sendNotRegistered(Client *client)
{
    sendReply(client, 451, ":You have not registered");
}

void IRCReplies::sendUnknownMode(Client *client, const std::string &mode)
{
    sendReply(client, 472, mode + " :is unknown mode char to me");
}

void IRCReplies::sendChannelIsFull(Client *client, const std::string &channel)
{
    sendReply(client, 471, channel + " :Cannot join channel (+l)");
}

void IRCReplies::sendInviteOnlyChan(Client *client, const std::string &channel)
{
    sendReply(client, 473, channel + " :Cannot join channel (+i)");
}

void IRCReplies::sendBadChannelKey(Client *client, const std::string &channel)
{
    sendReply(client, 475, channel + " :Cannot join channel (+k)");
}
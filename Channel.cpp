#include "Channel.hpp"
#include "Client.hpp"
#include <algorithm>

Channel::Channel(const std::string &name)
: _name(name), _hasTopic(false), _inviteOnly(false), _topicProtected(false), 
  _hasKey(false), _userLimit(0), _hasUserLimit(false)
{}

Channel::~Channel() {}

const std::string &Channel::getName() const
{
    return _name;
}

void Channel::addClient(Client *client)
{
    // add client if not already present
    if (!hasClient(client))
    {
        _clients.push_back(client);
        
        // First client becomes operator
        if (_clients.size() == 1)
        {
            addOperator(client);
        }
    }
}

void Channel::removeClient(Client *client)
{
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
    _operators.erase(std::remove(_operators.begin(), _operators.end(), client), _operators.end());
}

bool Channel::hasClient(Client *client) const
{
    return std::find(_clients.begin(), _clients.end(), client) != _clients.end();
}

void Channel::broadcastMessage(const std::string &msg, const Client *sender)
{
    std::cout << "DEBUG: Broadcasting message: " << msg << std::endl;
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if (_clients[i] != sender)
        {
            std::cout << "DEBUG: Sending to client " << _clients[i]->getFd() << std::endl;
            bool success = _clients[i]->sendMsgToClient(msg);
            std::cout << "DEBUG: Send result: " << (success ? "SUCCESS" : "FAILED") << std::endl;
        }
    }
}

std::vector<Client*> Channel::getClients() const
{
    return _clients;
}

// Topic methods
bool Channel::hasTopic() const
{
    return _hasTopic;
}

const std::string &Channel::getTopic() const
{
    return _topic;
}

void Channel::setTopic(const std::string &topic)
{
    _topic = topic;
    _hasTopic = !topic.empty();
}

// Operator methods
bool Channel::isOperator(Client *client) const
{
    return std::find(_operators.begin(), _operators.end(), client) != _operators.end();
}

void Channel::addOperator(Client *client)
{
    if (!isOperator(client))
    {
        _operators.push_back(client);
    }
}

void Channel::removeOperator(Client *client)
{
    _operators.erase(std::remove(_operators.begin(), _operators.end(), client), _operators.end());
}

// Channel mode methods
bool Channel::isInviteOnly() const
{
    return _inviteOnly;
}

void Channel::setInviteOnly(bool inviteOnly)
{
    _inviteOnly = inviteOnly;
}

bool Channel::isTopicProtected() const
{
    return _topicProtected;
}

void Channel::setTopicProtected(bool topicProtected)
{
    _topicProtected = topicProtected;
}

bool Channel::hasKey() const
{
    return _hasKey;
}

const std::string &Channel::getKey() const
{
    return _key;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
    _hasKey = true;
}

void Channel::removeKey()
{
    _key.clear();
    _hasKey = false;
}

bool Channel::hasUserLimit() const
{
    return _hasUserLimit;
}

int Channel::getUserLimit() const
{
    return _userLimit;
}

void Channel::setUserLimit(int limit)
{
    _userLimit = limit;
    _hasUserLimit = true;
}

void Channel::removeUserLimit()
{
    _userLimit = 0;
    _hasUserLimit = false;
}

std::string Channel::getModeString() const
{
    std::string modes = "+";
    
    if (_inviteOnly)
        modes += "i";
    if (_topicProtected)
        modes += "t";
    if (_hasKey)
        modes += "k";
    if (_hasUserLimit)
        modes += "l";
    
    if (modes == "+")
        return "";
    
    return modes;
}

// Invite list methods
void Channel::addToInviteList(Client *client)
{
    if (!isInvited(client)) {
        _inviteList.push_back(client);
    }
}

void Channel::removeFromInviteList(Client *client)
{
    _inviteList.erase(std::remove(_inviteList.begin(), _inviteList.end(), client), _inviteList.end());
}

bool Channel::isInvited(Client *client) const
{
    return std::find(_inviteList.begin(), _inviteList.end(), client) != _inviteList.end();
}
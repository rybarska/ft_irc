#include "Channel.hpp"
#include "Client.hpp"
#include <algorithm>

Channel::Channel(const std::string &name)
: _name(name)
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
        _clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
}

bool Channel::hasClient(Client *client) const
{
    return std::find(_clients.begin(), _clients.end(), client) != _clients.end();
}

void Channel::broadcastMessage(const std::string &msg, const Client *sender)
{
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if (_clients[i] != sender)
            _clients[i]->sendMessage(msg);
    }
}
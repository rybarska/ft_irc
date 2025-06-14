#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>

class Client; // forward declaration

class Channel
{
    public:
        Channel(const std::string &name);
        ~Channel();
        
        const std::string &getName() const;
        void addClient(Client *client);
        void removeClient(Client *client);
        bool hasClient(Client *client) const;
        void broadcastMessage(const std::string &msg, const Client *sender);
        
    private:
        std::string _name;
        std::vector<Client*> _clients;
};

#endif
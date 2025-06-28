#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>


class Client; 

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
        std::vector<Client*> getClients() const;

         // Topic methods
        bool hasTopic() const;
        const std::string &getTopic() const;
        void setTopic(const std::string &topic);
        
        // Operator methods
        bool isOperator(Client *client) const;
        void addOperator(Client *client);
        void removeOperator(Client *client);
        
        // Channel mode methods
        bool isInviteOnly() const;
        void setInviteOnly(bool inviteOnly);
        bool isTopicProtected() const;
        void setTopicProtected(bool topicProtected);
        bool hasKey() const;
        const std::string &getKey() const;
        void setKey(const std::string &key);
        void removeKey();
        bool hasUserLimit() const;
        int getUserLimit() const;
        void setUserLimit(int limit);
        void removeUserLimit();
        std::string getModeString() const;
        
        // Invite list methods
        void addToInviteList(Client *client);
        void removeFromInviteList(Client *client);
        bool isInvited(Client *client) const;
        
    private:
        std::string _name;
        std::vector<Client*> _clients;
        std::vector<Client*> _operators;
        std::string _topic;
        bool _hasTopic;
        
        // Channel modes
        bool _inviteOnly;
        bool _topicProtected;
        std::string _key;
        bool _hasKey;
        int _userLimit;
        bool _hasUserLimit;
        
        // Invite list
        std::vector<Client*> _inviteList;
};

#endif
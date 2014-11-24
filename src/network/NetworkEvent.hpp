#ifndef __NETWORK_EVENT_HPP__
#define __NETWORK_EVENT_HPP__

#include <SFML/Network.hpp>

#include "Event.hpp"
#include "Change.hpp"

struct NetworkEvent : public Event
{
    NetworkEvent();

    NetworkEvent(const unsigned int cid, 
                 const unsigned int sid, 
                 const Event &e);

    void fromPacket(sf::Packet &packet);
    void toPacket(sf::Packet &packet);

    Change toChange() const;

    unsigned int clientID;
    unsigned int stepID;
};

#endif /* __NETWORK_EVENT_HPP__ */


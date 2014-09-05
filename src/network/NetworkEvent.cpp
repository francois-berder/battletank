#include "NetworkEvent.hpp"


NetworkEvent::NetworkEvent():
Event(),
clientID(0),
stepID(0)
{
}

NetworkEvent::NetworkEvent(const unsigned int cid, 
                           const unsigned int sid, 
                           const Event &e):
Event(e),
clientID(cid),
stepID(sid)
{
}

void NetworkEvent::fromPacket(sf::Packet &packet)
{
    unsigned char t;
    packet >> clientID >> stepID >> t;
    type = static_cast<EventType>(t);
    if(type == EventType::Mouse)

        packet >> x >> y;
}

void NetworkEvent::toPacket(sf::Packet &packet)
{
    unsigned char t = static_cast<unsigned char>(type);
    packet << clientID << stepID << t;
    if(type == EventType::Mouse)
        packet << x << y;
}

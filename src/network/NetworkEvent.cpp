#include "NetworkEvent.hpp"
#include "Utils.hpp"

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

Change NetworkEvent::toChange() const
{
    std::string name;
    std::list<std::string> args;

    switch(type)
    {
        case EventType::Left:
            name = "move";
            args.push_back("left");
            break;
        case EventType::Right:
            name = "move";
            args.push_back("right");
            break;
        case EventType::Up:
            name = "move";
            args.push_back("up");
            break;
        case EventType::Down:
            name = "move";
            args.push_back("down");
            break;
        case EventType::Mouse:
            name = "dir";
            args.push_back(toString(x));
            args.push_back(toString(y));
            break;
        case EventType::Shoot:
            name = "shoot";
            break;
        default:
            throw std::runtime_error("Invalid type attribute of network event.");
    }

    return Change(clientID, name, args);
}

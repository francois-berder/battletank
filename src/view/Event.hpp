#ifndef __EVENT_H__
#define __EVENT_H__

enum class EventType
{
	Quit,
	Left,
	Right,
	Up,
	Down,
	Mouse
};

struct Event
{
    Event():
    type(EventType::Quit),
    x(-1),
    y(-1)
    {
    }

    Event(const EventType evtType):
    type(evtType),
    x(-1),
    y(-1)
    {
    }

    Event(const float posX, const float posY):
    type(EventType::Mouse),
    x(posX),
    y(posY)
    {
    }
    
    EventType type;
    float x;
    float y;
};

#endif /* __EVENT_H__ */


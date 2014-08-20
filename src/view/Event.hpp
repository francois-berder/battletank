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

class Event
{
    public :
    
        Event() = delete;
    
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
    
        virtual ~Event() = default;
    
        const EventType type;
        const float x;
        const float y;
};

#endif /* __EVENT_H__ */


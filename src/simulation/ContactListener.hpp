#ifndef __CONTACT_LISTENER_H__
#define __CONTACT_LISTENER_H__

#include <Box2D/Box2D.h>

class ContactListener : public b2ContactListener
{
	public:

		virtual void BeginContact(b2Contact *contact);
};

#endif /* __CONTACT_LISTENER_H__ */


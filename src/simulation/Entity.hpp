#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <memory>
#include <string>

class GameWorld;
class Change;

typedef unsigned int EntityID;

class Entity
{
	public:

		Entity(GameWorld &world, const EntityID id);
		virtual ~Entity();

		virtual void update();

		virtual void applyChange(const Change &change);

		virtual std::string print();

		EntityID getID() const;
        GameWorld& getWorld();

	private:

        GameWorld &m_world;
		const EntityID m_id;
};

typedef std::shared_ptr<Entity> EntityPtr;

#endif /* __ENTITY_H__ */


#ifndef __GAME_WORLD_H__
#define __GAME_WORLD_H__

#include <map>
#include <fstream>

#include "Entity.hpp"
#include "EntityFactory.hpp"
#include "Change.hpp"
#include "PhysicWorld.hpp"

class GameWorld
{
	public:

		GameWorld();

		void step();

		void applyChange(const Change &change);

		bool isFinished();

		std::string print() const;
		std::string printEntity(EntityID id) const;

		unsigned int getCurrentStep() const;

        void saveToFile(const std::string& fileName);

	private:

		void proceedChange(const std::string &name,
				const std::list<std::string> &args);

		unsigned int m_currentStep;
		std::map<EntityID, EntityPtr> m_entities;
        std::list<EntityPtr> m_toRemove;
		EntityFactory m_factory;
		std::ofstream m_saveFile;
};

#endif /* __GAME_WORLD_H__ */


#ifndef __CHANGE_H__
#define __CHANGE_H__

#include <string>
#include <list>

#include "Entity.hpp"
#include "NetworkEvent.hpp"

class Change
{
	public:

		Change() = delete;
		Change(const EntityID id, const std::string &name,
				const std::list<std::string> &args);

		EntityID getTargetID() const;
		std::string getName() const;
		std::list<std::string> getArgs() const;

        std::string toString() const;
        static Change fromString(const std::string &str);

	private:

		const EntityID m_id;
		const std::string m_name;
		const std::list<std::string> m_args;
};

#endif /* __CHANGE_H__ */


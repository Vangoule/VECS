#pragma once
#include "../core/manager.hpp"
#include "../component/component_manager.hpp"
#include "../core/events.hpp"
#include "../core/event_manager.hpp"
#include <deque>

namespace VECS
{
	class EntityManager : public Manager
	{
	public:
		EntityManager(EventManager* eventManager) : m_numEntities(0), m_eventManager(eventManager)
		{
			for (Entity ent = 0; ent < MAX_ENTITIES; ent++)
			{
				m_available.push_back(ent);
			}		
		}

		/*Returns a new unused Entity ID and reserves it until removal*/
		Entity create()
		{
			if (m_numEntities < MAX_ENTITIES)
			{
				Entity ent = m_available.front();
				m_available.pop_front();
				m_numEntities++;

				Events::EntityCreated e;
				e.entity = ent;
				m_eventManager->emitEvent(e);

				return ent;
			}

			return m_numEntities;
		}
	
		/*Frees up an entity ID and returns it to the pool*/
		bool remove(Entity ent)
		{
			if (ent < MAX_ENTITIES)
			{
				m_available.push_front(ent);
				m_numEntities--;

				Events::EntityRemoved e;
				e.entity = ent;
				m_eventManager->emitEvent(e);

				return true;
			}

			return false;
		}

		/*Returns the number of entities in use. */
		uint32_t getNumEntities()
		{
			return m_numEntities;
		}

		bool isValidEntity(Entity ent)
		{
			return ent < m_numEntities;
		}

	private:
		/*A list of all entity ID's not in use, uses a double ended queue to allow recovered ID's to be place at the front.*/
		std::deque<Entity> m_available;

		uint32_t m_numEntities;

		EventManager* m_eventManager;
	};
}
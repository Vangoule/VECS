#include "vecs/core/core.hpp"

namespace VECS
{
	ECS::ECS()
	{
		m_eventManager = new EventManager();

		m_systemManager = new SystemManager();
		m_systemManager->init(m_eventManager, m_universe);
	}

	Universe* ECS::createUniverse()
	{
		if (!m_universe)
		{
			m_universe = new Universe(m_eventManager);
			m_universe->create();
			m_systemManager->setUniverse(m_universe);
			return m_universe;
		}
		else
		{
			std::cerr << "Universe must be destroyed first" << std::endl;
			return nullptr;
		}
	}

	void ECS::destroyUniverse()
	{
		delete m_universe;
	}

	void ECS::resetUniverse()
	{

	}

	Universe* ECS::getUniverse()
	{
		return m_universe;
	}

	SystemManager* ECS::getSystemManager()
	{
		return m_systemManager;
	}

	EventManager* ECS::getEventManager()
	{
		return m_eventManager;
	}

	void ECS::tick()
	{
		//Process any actions on any components/entities
		m_universe->tick();

		//Update all systems, may generate new actions for the next tick.
		m_systemManager->tick();
	}
}
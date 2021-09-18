#include "vecs/core/universe.hpp"
namespace VECS {

	Universe::Universe(EventManager* eventManager) :
		m_entityManager(nullptr), m_componentManager(nullptr), m_actionQueue(nullptr), m_eventManager(eventManager)
	{
		std::cout << "Creating universe" << std::endl;
	}

	void Universe::create()
	{
		m_actionQueue = new ActionQueue();
		m_componentManager = new ComponentManager(m_eventManager);
		m_entityManager = new EntityManager(m_eventManager);
	}

	Entity Universe::createEntity()
	{
		return m_entityManager->create();
	}

	bool Universe::destroyEntity(Entity ent)
	{
		if (m_entityManager->isValidEntity(ent))
		{
			Action act([this, ent] { m_entityManager->remove(ent); });
			m_actionQueue->add(act);
			return true;
		}
		return false;
	}

	void Universe::tick()
	{
		m_actionQueue->processActions();
	}

}
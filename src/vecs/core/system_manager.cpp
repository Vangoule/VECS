#include "vecs/system/system_manager.hpp"
namespace VECS
{
	void SystemManager::registerSystem(System* system)
	{
		m_systems.push_back(system);
		system->setUniverse(m_universe);
		system->setEventManager(m_eventManager);
		system->init();
		
	}

	void SystemManager::removeSystem(System* system)
	{
		auto index = std::find(m_systems.begin(), m_systems.end(), system);
		m_systems.erase(index);
		system->shutdown();
	}

	void SystemManager::tick()
	{
		for (auto system : m_systems)
		{
			system->tick();
		}
	}
}
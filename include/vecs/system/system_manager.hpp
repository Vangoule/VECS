#pragma once
#include "vecs/core/universe.hpp"
#include "system.hpp"

#include <vector>

namespace VECS
{
	class SystemManager
	{
	public:
		void init(EventManager* eventManager, Universe* universe) { m_eventManager = eventManager; m_universe = universe; };

		void registerSystem(System* system);
		void removeSystem(System* system);

		void setUniverse(Universe* universe) { m_universe = universe; };

		void tick();

	private:
		std::vector<System*> m_systems;

		Universe* m_universe;
		EventManager* m_eventManager;

	};

}
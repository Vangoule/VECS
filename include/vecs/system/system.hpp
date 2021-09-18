#pragma once
#include "vecs/core/universe.hpp"
#include "vecs/core/event_manager.hpp"

namespace VECS
{
	class System
	{
	public:
		System() : m_universe(nullptr), m_eventManager(nullptr) {};
		virtual ~System() {};

		virtual void init() = 0;

		virtual void shutdown() = 0;

		virtual void tick() = 0;

		void setUniverse(Universe* universe) { m_universe = universe;  };
		void setEventManager(EventManager* eventManager) { m_eventManager = eventManager; };
	protected:
		Universe* m_universe;
		EventManager* m_eventManager;
	};
}
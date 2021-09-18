#pragma once
#include "../system/system_manager.hpp"
#include "event_manager.hpp"
#include "universe.hpp"

namespace VECS {
	class ECS {

	public:
		ECS();

		Universe* createUniverse();
		void destroyUniverse();
		void resetUniverse();


		Universe* getUniverse();
		SystemManager* getSystemManager();
		EventManager* getEventManager();


		void tick();

	private:
		SystemManager* m_systemManager;
		EventManager* m_eventManager;

		Universe* m_universe;
	};


}
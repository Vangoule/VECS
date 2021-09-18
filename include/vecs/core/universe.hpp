#pragma once
#include "common.hpp"

#include "../component/component_manager.hpp"
#include "../entity/entity_manager.hpp"

#include "../core/event_manager.hpp"


#include "action_queue.hpp"

namespace VECS
{
	class Universe
	{
	public:
		Universe(EventManager* eventManager);

		void create();

		void tick();

		template<isComponentType ComponentType>
		bool attachComponent(Entity ent, ComponentType comp);

		template<isComponentType ComponentType>
		bool detachComponent(Entity ent);

		Entity createEntity();
		bool destroyEntity(Entity ent);
		bool isValidEntity(Entity ent) { return m_entityManager->isValidEntity(ent); };

		template<isComponentType ComponentType>
		bool hasComponent(Entity ent);

		template<isComponentType ComponentTypeOne, isComponentType ComponentTypeTwo, isComponentType... ComponentTypes>
		bool hasComponent(Entity ent);

		template<isComponentType ...ComponentTypes>
		std::vector<Entity> entitiesWith();

		template<isComponentType ComponentType>
		void eachComponent(typename std::common_type<std::function<void(ComponentHandle<ComponentType>)>>::type func);

		template<isComponentType... ComponentTypes>
		void eachEntity(typename std::common_type<std::function<void(ComponentHandle<ComponentTypes>...)>>::type func);

		template<isComponentType... ComponentTypes>
		void eachEntity(typename std::common_type<std::function<void(Entity ent, ComponentHandle<ComponentTypes>...)>>::type func);

		void run(typename std::common_type<std::function<void(Entity ent)>>::type func);


		ComponentManager* getComponentManager() { return m_componentManager; }

	private:
		/*Actions*/
		ActionQueue* m_actionQueue;
		/*Entities*/
		EntityManager* m_entityManager;
		/*Components*/
		ComponentManager* m_componentManager;
		/*Events*/
		EventManager* m_eventManager;
	};

	/*Creates an action and adds it to the queue that attaches a component to an entity,
	  will return false if the entity is invalid.*/
	template<isComponentType ComponentType>
	inline bool Universe::attachComponent(Entity ent, ComponentType comp)
	{
		if (m_entityManager->isValidEntity(ent))
		{
			Action act([this, ent, comp] { m_componentManager->attachComponent(ent, comp); });
			m_actionQueue->add(act);
			return true;
		}
		return false;

	}

	/*Creates an action and adds it to the queue that detaches a component from an entity,
	  will return false if the entity is invalid.*/
	template<isComponentType ComponentType>
	inline bool Universe::detachComponent(Entity ent)
	{
		if (m_entityManager->isValidEntity(ent))
		{
			Action act([this, ent] { m_componentManager->detachComponent<ComponentType>(ent); });
			m_actionQueue->add(act);
			return true;
		}
		return false;
	}

	/*Returns a vector of entities containg the required components*/
	template<isComponentType ...ComponentTypes>
	inline std::vector<Entity> Universe::entitiesWith()
	{
		std::vector<Entity> entities;
		for (Entity ent = 0; ent < m_entityManager->getNumEntities(); ent++)
		{
			if (hasComponent<ComponentTypes...>(ent))
				entities.push_back(ent);
		}
		return entities;
	}

	/*Runs a function on each component directly*/
	template<isComponentType ComponentType>
	inline void Universe::eachComponent(std::common_type<std::function<void(ComponentHandle<ComponentType>)>>::type func)
	{
		auto componentStorage = m_componentManager->getComponentStorage<ComponentType>();
		auto entityComponentMap = componentStorage.getEntityComponentMap();
		auto components = componentStorage.getComponents();

		for (Entity ent = 0; ent < m_entityManager->getNumEntities(); ent++)
		{
			auto comp = &components[entityComponentMap[ent]];
			auto handle = ComponentHandle<ComponentType>(comp);
			func(handle);
		}

	}

	/*Runs a function on each entity, getting the component from the entity ID, slower than direct component access.*/
	template<isComponentType ...ComponentTypes>
	inline void Universe::eachEntity(std::common_type<std::function<void(ComponentHandle<ComponentTypes>...)>>::type func)
	{
		for (auto ent : entitiesWith<ComponentTypes...>())
		{
			func(m_componentManager->getHandleFromEntity<ComponentTypes>(ent)...);
		}

	}

	/*Runs a function on each entity, getting the component from the entity ID, slower than direct component access.*/
	template<isComponentType ...ComponentTypes>
	inline void Universe::eachEntity(std::common_type<std::function<void(Entity ent, ComponentHandle<ComponentTypes>...)>>::type func)
	{
		for (auto ent : entitiesWith<ComponentTypes...>())
		{
			func(ent, m_componentManager->getHandleFromEntity<ComponentTypes>(ent)...);
		}
	}

	inline void Universe::run(std::common_type<std::function<void(Entity ent)>>::type func)
	{
		for (Entity ent = 0; ent < m_entityManager->getNumEntities(); ent++)
		{
			func(ent);
		}
	}

	template<isComponentType ComponentType>
	inline bool Universe::hasComponent(Entity ent)
	{
		return m_componentManager->getHandleFromEntity<ComponentType>(ent);
	}

	template<isComponentType ComponentTypeOne, isComponentType ComponentTypeTwo, isComponentType... ComponentTypes>
	inline bool Universe::hasComponent(Entity ent)
	{
		return hasComponent<ComponentTypeOne>(ent) && hasComponent<ComponentTypeTwo, ComponentTypes...>(ent);
	}


}

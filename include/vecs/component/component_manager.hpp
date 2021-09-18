#pragma once
#include "../core/common.hpp"
#include "../core/manager.hpp"
#include "../core/event_manager.hpp"

#include "component.hpp"
#include "component_handle.hpp"
#include "component_storage.hpp"

#include <concepts>
#include <type_traits>
#include <typeindex>

namespace VECS
{
	/*Components are simple POD types, the Component Manager takes care of any functionality.*/
	class ComponentManager : public Manager
	{

	public:

		ComponentManager(EventManager* eventManager) : m_eventManager(eventManager) {

		}

		/*Returns a component from the storage for a given entity/component pair*/
		template <isComponentType ComponentType>
		ComponentHandle<ComponentType> getHandleFromEntity(Entity ent) {
			ComponentHandle<ComponentType> comp = getComponentStorage<ComponentType>().getFromEntity(ent);
			return comp;
		}

		/*Attach a component, returns a handle to the stored data if successful, a null handle otherwise.*/
		template <isComponentType ComponentType>
		ComponentHandle<ComponentType> attachComponent(Entity ent, ComponentType comp)
		{
			ComponentHandle<ComponentType> handle = getComponentStorage<ComponentType>().attach(ent, comp);

			Events::ComponentAttached<ComponentType> e;
			e.entity = ent;
			e.component = handle;
			m_eventManager->emitEvent(e);

			return handle;
		}

		/*Detach a component, returns true of component exists*/
		template <isComponentType ComponentType>
		bool detachComponent(Entity ent)
		{

			Events::ComponentDetached<ComponentType> e;
			e.entity = ent;
			e.component = getHandleFromEntity<ComponentType>(ent);
			
			bool removed = getComponentStorage<ComponentType>().detach(ent);

			if(removed)
				m_eventManager->emitEvent(e);

			return removed;
		}

		template <isComponentType ComponentType>
		bool detachComponent(Entity ent, ComponentType comp)
		{
			detachComponent<ComponentType>(ent);
		}

		/*Returns the component storage object for a given component type, this function will create each
		  storage the first time it is called, keep in mind this is not thread safe before C++11 due to
		  initialisation changes.*/
		template <isComponentType ComponentType>
		ComponentStorage<ComponentType>& getComponentStorage() {
			static ComponentStorage<ComponentType> m_components;
			return m_components;
		}

	private:
		EventManager* m_eventManager;
	};


}
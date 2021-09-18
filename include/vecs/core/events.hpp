#pragma once
#include "event.hpp"
#include "../entity/entity.hpp"
#include "../component/component.hpp"
namespace VECS
{
	namespace Events
	{
		struct EntityCreated : public Event
		{
			Entity entity;
		};

		struct EntityRemoved : public Event
		{
			Entity entity;
		};

		template<isComponentType ComponentType>
		struct ComponentAttached : public Event
		{
			Entity entity;
			ComponentHandle<ComponentType> component;
		};

		template<isComponentType ComponentType>
		struct ComponentDetached : public Event
		{
			Entity entity;
			ComponentHandle<ComponentType> component;
		};
	}
}
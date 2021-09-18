#pragma once
#include "../core/common.hpp"

#include "component_handle.hpp"

namespace VECS
{
	/* A component storage stores a map of components against entities. This allows for
	  quick lookup of specific components of a type as they will all be stored tohgether.*/
	template<isComponentType ComponentType>
	class ComponentStorage
	{
	public:
		ComponentHandle<ComponentType> getFromEntity(Entity ent)
		{
			if (hasComponent(ent))
			{
				uint32_t index = m_entityComponentMap[ent];
				ComponentType* comp = &m_components[index];
				return ComponentHandle<ComponentType>(comp);
			}
			return ComponentHandle<ComponentType>(nullptr);
		}



		/*Check if component of type exists on the entity already*/
		bool hasComponent(Entity ent)
		{
			if (m_entityComponentMap.find(ent) != m_entityComponentMap.end())
			{
				return true;
			};
			return false;
		}

		/* Attaches a component to an entity by mapping the ID to the component index. */
		ComponentHandle<ComponentType> attach(Entity ent, ComponentType comp)
		{
			if (hasComponent(ent))
			{
				std::cout << "A component of that type is already attached to this entity." << std::endl;
				return ComponentHandle<ComponentType>(nullptr);
			};

			/*Add the component to the array*/
			uint32_t nextComponentSlot = m_numComponents;
			m_components[nextComponentSlot] = comp;

			/*Point the entity map to the new component*/
			m_entityComponentMap[ent] = nextComponentSlot;

			/*Point the component map to the entity*/
			m_componentEntityMap[nextComponentSlot] = ent;

			m_numComponents++;

			return ComponentHandle<ComponentType>(&m_components[nextComponentSlot]);
		}

		/* Detaches the component from an entity. */
		bool detach(Entity ent)
		{
			if (!hasComponent(ent))
			{
				std::cout << "The component doesn't have an entity of this type." << std::endl;
				return false;
			};

			//Erase the entity from the map
			uint32_t index = m_entityComponentMap[ent];
			m_components[index] = m_components[m_numComponents - 1];

			/*Replace the detached component with data from the end of the array*/
			Entity last = m_componentEntityMap[m_numComponents - 1];
			m_entityComponentMap[last] = index;
			m_componentEntityMap[index] = last;

			/*Erase the component from both maps*/
			m_entityComponentMap.erase(ent);
			m_componentEntityMap.erase(last);

			m_numComponents--;

			return true;

		}

		/*Map entity ids to component ids*/
		std::unordered_map<Entity, uint32_t>& getEntityComponentMap()
		{
			return m_entityComponentMap;
		}

		std::array<ComponentType, MAX_ENTITIES>& getComponents()
		{
			return m_components;
		}

	private:
		/*Map components ids to entity ids*/
		std::unordered_map<uint32_t, Entity> m_componentEntityMap;

		/*Map entity ids to component ids*/
		std::unordered_map<Entity, uint32_t> m_entityComponentMap;

		/*Keep components of seperate types together in contiguous memory for fast access.*/
		std::array<ComponentType, MAX_ENTITIES> m_components;

		uint32_t m_numComponents;

	};

}
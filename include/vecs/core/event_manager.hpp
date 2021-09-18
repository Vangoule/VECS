#pragma once

#include "event.hpp"
#include "../core/manager.hpp"

#include <iostream>
#include <stdint.h>
#include <vector>
#include <unordered_map>

#include <functional>
#include <typeindex>

namespace VECS
{
	class Listener
	{
	public:
		virtual ~Listener() {};
	};

	template<isEventType EventType>
	class EventListener : public Listener
	{
	public:
		virtual ~EventListener() {}

		virtual void handleEvent(const EventType& event) = 0;
	};



	class EventManager : public Manager
	{
	public:
		/*Adds an event listener by pointer to base class.*/
		template<isEventType EventType>
		void addEventListener(EventListener<EventType>* listener)
		{
			auto type = std::type_index(typeid(EventType));
			auto found = m_eventListeners.find(type);
			if (found == m_eventListeners.end())
			{
				std::vector<Listener*> listeners;
				listeners.push_back(listener);

				m_eventListeners.insert({ type, listeners });
			}
			else
			{
				found->second.push_back(listener);
			}
		}

		/*Removes an event listener by pointer, returns true if sucessfull*/
		template<isEventType EventType>
		inline bool removeEventListener(EventListener<EventType>* listener)
		{
			auto type = std::type_index(typeid(EventType));
			auto found = m_eventListeners.find(type);
			if (found != m_eventListeners.end())
			{
				found->second.erase(std::remove(found->second.begin(), found->second.end(), listener), found->second.end());
				if (found->second.size() == 0)
				{
					m_eventListeners.erase(found);
					return true;
				}
			}
			return false;
		}


		typedef std::function<void(void*)> EventFunction;

		/*Returns the position in the function storage vector to facilitate removal of generic lambda functions*/
		template<isEventType EventType>
		uint32_t addEventListener(const std::function<void(EventType)>& func)
		{
			auto type = std::type_index(typeid(EventType));
			auto found = m_eventFunctions.find(type);
			if (found == m_eventFunctions.end())
			{
				std::vector<EventFunction> functions;
				functions.push_back([func](void* event) { func(*(EventType*)event); });

				m_eventFunctions.insert({ type, functions });
				return 0;
			}
			else
			{
				uint32_t size = found->second.size();
				found->second.push_back([func](void* event) { func(*(EventType*)event); });
				return size;
			}
		}

		/*Removes an event listener by ID, returns true if successfull, necessary for Lambdas.*/
		template<isEventType EventType>
		inline bool removeEventListener(uint32_t eventHandlerID)
		{
			auto type = std::type_index(typeid(EventType));
			auto found = m_eventFunctions.find(type);
			if (found != m_eventFunctions.end())
			{
				found->second.erase(std::next(found->second.begin(), eventHandlerID));
				if (found->second.size() == 0)
				{
					m_eventFunctions.erase(found);
					return true;
				}
			}
			return false;
		}

		/*Instantly emit an event, calling each listener outside of the normal order*/
		template<isEventType EventType>
		void emitEvent(const EventType& event)
		{
			auto found = m_eventListeners.find(std::type_index(typeid(EventType)));
			if (found != m_eventListeners.end())
			{
				for (auto* listener : found->second)
				{
					auto* eventListener = reinterpret_cast<EventListener<EventType>*>(listener);
					eventListener->handleEvent(event);

				}
			}

			auto funcFound = m_eventFunctions.find(std::type_index(typeid(EventType)));
			if (funcFound != m_eventFunctions.end())
			{
				for (auto func : funcFound->second)
				{
					func((void*)&event);
				}
			}
		}

	private:

		std::unordered_map<std::type_index, std::vector<EventFunction>, std::hash<std::type_index>, std::equal_to<std::type_index>> m_eventFunctions;
		std::unordered_map<std::type_index, std::vector<Listener*>, std::hash<std::type_index>, std::equal_to<std::type_index>> m_eventListeners;
	};

}
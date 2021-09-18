#pragma once

namespace VECS
{
	/*A handle for components which wraps a component and gives it more functionality.*/
	template<typename ComponentType>
	class ComponentHandle
	{
	public:
		ComponentHandle()
			: m_component(nullptr)
		{
		}

		ComponentHandle(ComponentType* component)
			: m_component(component)
		{

		}

		ComponentType* operator->() const
		{
			return m_component;
		}

		ComponentType& component()
		{
			return *m_component;
		}

		bool isNullPtr() const
		{
			return m_component == nullptr;
		}

		operator bool() const
		{
			return !isNullPtr();
		}

	private:
		ComponentType* m_component;
	};
}
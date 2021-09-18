#pragma once

namespace VECS
{
	/*Data is stored in static arrays for fast access, this should hopefully be enough.*/
	constexpr uint32_t MAX_COMPONENTS = 64;

	/*Components are just simple POD, the component class facilitates type-safety and storage for derived components.*/
	class Component
	{
	public:
		virtual ~Component() {};
	};

	/*C++20 Concept to ensure type-safety with templates, will ensure any types are derived from the base class component.*/
	template<typename T>
	concept isComponentType = requires(T v) {
		{ v } -> std::convertible_to<Component>;
	};
}
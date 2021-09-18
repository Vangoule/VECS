#pragma once

namespace VECS
{
	class Event
	{
	public:
	};

	template<typename T>
	concept isEventType = requires(T v) {
		{ v } -> std::convertible_to<Event>;
	};

}
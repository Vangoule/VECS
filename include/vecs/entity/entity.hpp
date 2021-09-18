#pragma once
#include "../core/common.hpp"

namespace VECS
{
	//Entities are a simple POD type.
	using Entity = uint32_t;

	//Data is stored in static arrays for fast access, this should hopefully be enough.
	const uint32_t MAX_ENTITIES = 10000;
}

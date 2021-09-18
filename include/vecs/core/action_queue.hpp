#pragma once

#include "action.hpp"

#include <iostream>
#include <queue>

namespace VECS
{
	class ActionQueue 
	{
	public:
		ActionQueue();

		void add(const Action& action);
		void add(Action&& action);
		void processActions();

	private:
		std::queue<Action> m_actionQueue;
	};

}
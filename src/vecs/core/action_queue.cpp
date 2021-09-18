#include "vecs/core/action_queue.hpp"

namespace VECS {
	ActionQueue::ActionQueue()
	{
		
	}

	void ActionQueue::add(const Action& action)
	{
		m_actionQueue.push(action);
	}

	void ActionQueue::add(Action&& action)
	{
		m_actionQueue.push(std::move(action));
	}

	void ActionQueue::processActions()
	{
		while (m_actionQueue.size() > 0)
		{
			auto act = std::move(m_actionQueue.front());

			act.call();

			m_actionQueue.pop();
		}
	}
}
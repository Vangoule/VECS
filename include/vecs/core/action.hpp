#pragma once

#include <chrono>
#include <functional>

namespace VECS
{
	using ActionTime = std::chrono::time_point<std::chrono::system_clock>;
	typedef std::function<void(void)> ActionFunction;

	class Action {
	public:
		Action(const ActionFunction& act) { m_time = std::chrono::system_clock::now(); m_actionFunction = act; }
		Action(ActionFunction&& act) { m_time = std::chrono::system_clock::now(); m_actionFunction = std::move(act); }

		void setTime(ActionTime time) { m_time = time; }
		ActionTime getTime() { return m_time; }
		void call() { m_actionFunction(); };

	private:
		ActionFunction m_actionFunction;
		ActionTime m_time;
	};

}
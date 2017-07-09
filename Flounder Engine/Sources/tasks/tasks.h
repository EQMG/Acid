#pragma once

#include <iostream>
#include <vector>
#include "../framework/framework.h"
#include "itask.h"

namespace flounder {
	class tasks : public module
	{
	private:
		std::vector<itask*> *m_tasks;
	public:
		static tasks *get()
		{
			return (tasks*) framework::get()->getInstance("tasks");
		}

		tasks();

		~tasks();

		void update();

		/// <summary>
		/// Adds an task to the que.
		/// </summary>
		/// <param name="task"> The task to add. </param>
		inline void addTask(itask *task);

		/// <summary>
		/// Removes a task from the que.
		/// </summary>
		/// <param name="task"> The task to remove. </param>
		inline void removeTask(itask *task);
	};
}

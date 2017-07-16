#pragma once

#include <iostream>
#include <functional>
#include <vector>

#include "../framework/framework.h"

namespace flounder {
	class tasks :
		public imodule
	{
	private:
		std::vector<std::function<void()>> *m_tasks;
	public:
		static tasks *get()
		{
			return (tasks*) framework::get()->getInstance("tasks");
		}

		tasks();

		~tasks();

		void init();

		void update();

		/// <summary>
		/// Adds an task to the que.
		/// </summary>
		/// <param name="task"> The task to add. </param>
		void addTask(std::function<void()> task);
	};
}

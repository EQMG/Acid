#pragma once

#include <iostream>
#include <functional>
#include <vector>

#include "../framework/framework.h"

namespace flounder
{
	/// <summary>
	/// A module used for managing tasks on framework updates.
	/// </summary>
	class tasks :
		public imodule
	{
	private:
		std::vector<std::function<void()>> *m_tasks;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline tasks *get()
		{
			return static_cast<tasks*>(framework::get()->getInstance("tasks"));
		}

		tasks();

		~tasks();

		void update() override;

		/// <summary>
		/// Adds an task to the que.
		/// </summary>
		/// <param name="task"> The task to add. </param>
		void addTask(std::function<void()> task);
	};
}

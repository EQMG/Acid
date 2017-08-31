#pragma once

#include <iostream>
#include <functional>
#include <vector>

#include "../engine/Engine.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing tasks on engine updates.
	/// </summary>
	class tasks :
		public IModule
	{
	private:
		std::vector<std::function<void()>> *m_tasks;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline tasks *get()
		{
			return static_cast<tasks*>(Engine::Get()->GetModule("tasks"));
		}

		tasks();

		~tasks();

		void Update() override;

		/// <summary>
		/// Adds an task to the que.
		/// </summary>
		/// <param name="task"> The task to add. </param>
		void addTask(std::function<void()> task);
	};
}

#pragma once

#include <functional>
#include <vector>
#include "../Engine/Engine.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing tasks on engine updates.
	/// </summary>
	class F_EXPORT Tasks :
		public IModule
	{
	private:
		std::vector<std::function<void()>> *m_tasks;
	public:
		static Tasks *S_INSTANCE;

		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Tasks *Get()
		{
			return S_INSTANCE; // reinterpret_cast<Tasks *>(Engine::Get()->GetModule("tasks"));
		}

		Tasks();

		~Tasks();

		void Update() override;

		/// <summary>
		/// Adds an task to the que.
		/// </summary>
		/// <param name="task"> The task to add. </param>
		void AddTask(std::function<void()> task) const;
	};
}

#pragma once

#include <vector>
#include <memory>
#include "Maths/Timer.hpp"
#include "Engine/Engine.hpp"
#include "IResource.hpp"

namespace fl
{
	/// <summary>
	/// A module used for managing resources.
	/// </summary>
	class FL_EXPORT Resources :
		public IModule
	{
	private:
		std::vector<std::shared_ptr<IResource>> m_resources;
		Timer m_timerPurge;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Resources *Get()
		{
			return Engine::Get()->GetModule<Resources>();
		}

		/// <summary>
		/// Creates a new Resources module.
		/// </summary>
		Resources();

		/// <summary>
		/// Deconstructor for the Resources module.
		/// </summary>
		~Resources();

		void Update() override;

		std::shared_ptr<IResource> Get(const std::string &filename);

		void Add(std::shared_ptr<IResource> resource);

		void Remove(std::shared_ptr<IResource> resource);

		void Remove(const std::string &filename);
	};
}

#pragma once

#include <vector>
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
		std::vector<IResource *> *m_managed;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Resources *Get()
		{
			return reinterpret_cast<Resources *>(Engine::Get()->GetModule("resources"));
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

		IResource *Get(const std::string &filename);

		void Add(IResource *managed);

		void Remove(IResource *managed);

		void Remove(const std::string &filename);
	};
}

#pragma once

#include <vector>
#include "../Engine/Engine.hpp"
#include "IStandard.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing simple update injection standards.
	/// </summary>
	class Standards :
		public IModule
	{
	private:
		std::vector<IStandard*> *m_standards;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Standards *get()
		{
			return static_cast<Standards*>(Engine::Get()->GetModule("standards"));
		}

		Standards();

		~Standards();

		void AddStandard(IStandard *standard) const;

		void Update() override;
	};
}

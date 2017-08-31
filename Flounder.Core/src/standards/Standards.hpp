#pragma once

#include <vector>

#include "../engine/Engine.hpp"

#include "IStandard.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing simple update injection standards.
	/// </summary>
	class standards :
		public IModule
	{
	private:
		std::vector<IStandard*> *m_standards;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static standards *get()
		{
			return static_cast<standards*>(Engine::Get()->GetModule("standards"));
		}

		standards();

		~standards();

		void addStandard(IStandard *standard);

		void Update() override;
	};
}

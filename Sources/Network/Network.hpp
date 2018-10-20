#pragma once

#include "Engine/Engine.hpp"

namespace acid
{
	/// <summary>
	/// A module used for managing networked servers and clients.
	/// </summary>
	class ACID_EXPORT Network :
		public IModule
	{
	private:
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Network *Get() { return Engine::Get()->GetModule<Network>(); }

		Network();

		void Update() override;
	};
}

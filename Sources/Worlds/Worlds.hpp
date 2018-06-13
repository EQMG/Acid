#pragma once

#include "Engine/Engine.hpp"
#include "IWorld.hpp"

namespace fl
{
	/// <summary>
	/// A module used for managing worlds.
	/// </summary>
	class FL_EXPORT Worlds :
		public IModule
	{
	private:
		IWorld *m_world;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Worlds *Get()
		{
			return reinterpret_cast<Worlds *>(Engine::Get()->GetModule("worlds"));
		}

		/// <summary>
		/// Creates a new worlds module.
		/// </summary>
		Worlds();

		/// <summary>
		/// Deconstructor for the worlds module.
		/// </summary>
		~Worlds();

		void Update() override;

		IWorld *GetWorld() const { return m_world; }

		void SetWorld(IWorld *world);
	};
}

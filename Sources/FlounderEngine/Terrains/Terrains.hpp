#pragma once

#include <vector>
#include "../Engine/Engine.hpp"
#include "../Maths/Noise/NoiseFast.hpp"
#include "TerrainRender.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing terrains in 3D worlds.
	/// </summary>
	class F_EXPORT Terrains :
		public IModule
	{
	private:
		NoiseFast m_noise1;
	public:
		static Terrains *S_INSTANCE;

		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Terrains *Get()
		{
			return S_INSTANCE; // reinterpret_cast<Terrains *>(Engine::Get()->GetModule("terrains"));
		}

		/// <summary>
		/// Creates a new terrains module.
		/// </summary>
		Terrains();

		/// <summary>
		/// Deconstructor for the terrains module.
		/// </summary>
		~Terrains();

		void Update() override;

		float GetHeight(const float &x, const float &z);

		Vector3 GetNormal(const float &x, const float &z);

		Vector3 GetPosition(const float &x, const float &z);
	};
}

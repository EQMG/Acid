#pragma once

#include "../Engine/Engine.hpp"
#include "../Lights/Fog.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing skyboxes in 3D worlds.
	/// </summary>
	class F_EXPORT Skyboxes :
		public IModule
	{
	private:
		Fog *m_fog;
		Colour *m_skyColour;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Skyboxes *Get()
		{
			return reinterpret_cast<Skyboxes *>(Engine::Get()->GetModule("skyboxes"));
		}

		Skyboxes();

		~Skyboxes();

		void Update() override;

		Fog *GetFog() const { return m_fog; }

		void SetFog(const Fog &fog) { *m_fog = fog; }

		Colour *GetSkyColour() const { return m_skyColour; }

		void SetSkyColour(const Colour &skyColour) { *m_skyColour = skyColour; }
	};
}

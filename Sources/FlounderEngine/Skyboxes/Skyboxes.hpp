#pragma once

#include "../Engine/Engine.hpp"
#include "../Lights/Fog.hpp"
#include "Skybox.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing skyboxes in 3D worlds.
	/// </summary>
	class Skyboxes :
		public IModule
	{
	private:
		Skybox *m_skybox;
		Fog *m_fog;
		Colour *m_skyColour;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Skyboxes *Get()
		{
			return dynamic_cast<Skyboxes *>(Engine::Get()->GetModule("skyboxes"));
		}

		Skyboxes();

		~Skyboxes();

		void Update() override;

		Skybox *GetSkybox() const { return m_skybox; }

		void SetSkybox(Skybox *skybox) { m_skybox = skybox; }

		Fog *GetFog() const { return m_fog; }

		void SetFog(const Fog &fog) { m_fog->Set(fog); }

		Colour *GetSkyColour() const { return m_skyColour; }

		void SetSkyColour(const Colour &skyColour) { m_skyColour->Set(skyColour); }
	};
}

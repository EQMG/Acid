#pragma once

#include "../engine/Engine.hpp"
#include "../lights/Fog.hpp"
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
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Skyboxes *Get()
		{
			return static_cast<Skyboxes*>(Engine::Get()->GetModule("skyboxes"));
		}

		Skyboxes();

		~Skyboxes();

		void Update() override;

		Skybox *GetSkybox() const { return m_skybox; }

		void SetSkybox(Skybox *skybox) { m_skybox = skybox; }

		Fog *GetFog() const { return m_fog; }

		void SetFog(Fog *fog) { m_fog = fog; }
	};
}

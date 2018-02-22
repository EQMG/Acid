#pragma once

#include "../Engine/Engine.hpp"
#include "../Objects/GameObject.hpp"
#include "../Space/StructureBasic.hpp"
#include "Scene.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing game scenes on engine updates.
	/// </summary>
	class F_EXPORT Scenes :
		public IModule
	{
	private:
		Scene *m_scene;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Scenes *Get()
		{
			return reinterpret_cast<Scenes *>(Engine::Get()->GetModule("scenes"));
		}

		/// <summary>
		/// Creates a new Scenes module.
		/// </summary>
		Scenes();

		/// <summary>
		/// Deconstructor for the Scenes module.
		/// </summary>
		~Scenes();

		void Update() override;

		StructureBasic<GameObject *> *GetStructure() const { return m_scene->GetStructure(); }

		Scene *GetScene() const { return m_scene; }

		void SetScene(Scene *scene) { m_scene = scene; }
	};
}

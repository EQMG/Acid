#pragma once

#include "../Engine/Engine.hpp"
#include "../Objects/GameObject.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing game scenes on engine updates.
	/// </summary>
	class F_EXPORT Scenes :
		public IModule
	{
	private:
		ISpatialStructure<GameObject *> *m_structure;
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

		ISpatialStructure<GameObject *> *GetStructure() const { return m_structure; }

		void SetStructure(ISpatialStructure<GameObject *> *structure) { m_structure = structure; }
	};
}

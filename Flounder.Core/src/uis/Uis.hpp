#pragma once

#include "../engine/Engine.hpp"
#include "../fonts/Text.hpp"
#include "../guis/Gui.hpp"
#include "IManagerUis.hpp"
#include "UiSelector.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for that manages gui textures in a container.
	/// </summary>
	class Uis :
		public IModule
	{
	private:
		IManagerUis *m_managerUis;
		UiSelector *m_selector;
		UiObject *m_container;
		std::vector<UiObject*> *m_objects;
	public:
		FontType *m_arial;
		FontType *m_berlinSans;
		FontType *m_cafeFrancoise;
		FontType *m_candara;
		FontType *m_segoe;

		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Uis *Get()
		{
			return static_cast<Uis*>(Engine::Get()->GetModule("uis"));
		}

		/// <summary>
		/// Creates a new uis module.
		/// </summary>
		Uis();

		/// <summary>
		/// Deconstructor for the uis module.
		/// </summary>
		~Uis();

		void Update() override;

		/// <summary>
		/// Gets the uis manager.
		/// </summary>
		/// <returns> The uis manager. </returns>
		IManagerUis *GetManager() const { return m_managerUis; };

		/// <summary>
		/// Sets the current uis manager to a new uis manager.
		/// </summary>
		/// <param name="managerUis"> The new uis manager. </param>
		void SetManager(IManagerUis *managerUis) { m_managerUis = managerUis; }

		/// <summary>
		/// Gets the screen container.
		/// </summary>
		/// <returns> The screen container. </returns>
		UiObject *GetContainer() const { return m_container; }

		/// <summary>
		/// Gets the main GUI selector.
		/// </summary>
		/// <returns> The GUI selector. </returns>
		UiSelector *GetSelector() const { return m_selector; }

		/// <summary>
		/// The rendering objects from the container. Updated each update.
		/// </summary>
		/// <returns> The objects. </returns>
		std::vector<UiObject*> *GetObjects() const { return m_objects; };
	};
}

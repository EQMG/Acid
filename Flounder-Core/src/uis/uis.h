#pragma once

#include "../framework/framework.h"
#include "../fonts/text.h"
#include "../guis/gui.h"

#include "iuimanager.h"
#include "uiselector.h"
#include "containerscreen.h"

namespace flounder
{
	/// <summary>
	/// A module used for that manages gui textures in a container.
	/// </summary>
	class uis : 
		public imodule
	{
	private:
		iuimanager *m_uiManager;
		uiselector *m_selector;
		uiobject *m_container;
		std::vector<uiobject*> *m_objects;
	public:
		fonttype *arial;
		fonttype *berlinSans;
		fonttype *cafeFrancoise;
		fonttype *candara;
		fonttype *segoe;

		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static uis *get()
		{
			return static_cast<uis*>(framework::get()->getInstance("uis"));
		}

		/// <summary>
		/// Creates a new uis module.
		/// </summary>
		uis();

		/// <summary>
		/// Deconstructor for the uis module.
		/// </summary>
		~uis();

		/// <summary>
		/// Loads the ui module with inital values.
		/// </summary>
		/// <param name="uiManager"> The inital ui master. </param>
		void load(iuimanager *uiManager);

		void init() override;

		void update() override;

		/// <summary>
		/// Gets the ui manager.
		/// </summary>
		/// <returns> The ui manager. </returns>
		inline iuimanager *getUiManager() { return m_uiManager; };

		/// <summary>
		/// Gets the screen container.
		/// </summary>
		/// <returns> The screen container. </returns>
		inline uiobject *getContainer() { return m_container; }

		/// <summary>
		/// Gets the main GUI selector.
		/// </summary>
		/// <returns> The GUI selector. </returns>
		inline uiselector *getSelector() { return m_selector; }

		/// <summary>
		/// The rendering objects from the container. Updated each update.
		/// </summary>
		/// <returns> The objects. </returns>
		inline std::vector<uiobject*> *getObjects() { return m_objects; };
	};
}

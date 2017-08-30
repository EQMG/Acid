#pragma once

#include "../framework/framework.hpp"
#include "../fonts/text.hpp"
#include "../guis/gui.hpp"

#include "imanageruis.hpp"
#include "uiselector.hpp"
#include "containerscreen.hpp"

namespace flounder
{
	/// <summary>
	/// A module used for that manages gui textures in a container.
	/// </summary>
	class uis :
		public imodule
	{
	private:
		imanageruis *m_managerUis;
		uiselector *m_selector;
		uiobject *m_container;
		std::vector<uiobject*> *m_objects;
	public:
		fonttype *m_arial;
		fonttype *m_berlinSans;
		fonttype *m_cafeFrancoise;
		fonttype *m_candara;
		fonttype *m_segoe;

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

		void update() override;

		/// <summary>
		/// Gets the uis manager.
		/// </summary>
		/// <returns> The uis manager. </returns>
		inline imanageruis *getManager() { return m_managerUis; };

		/// <summary>
		/// Sets the current uis manager to a new uis manager.
		/// </summary>
		/// <param name="managerUis"> The new uis manager. </param>
		inline void setMaster(imanageruis *managerUis) { m_managerUis = managerUis; }

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

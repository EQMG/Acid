#pragma once

#include "../framework/framework.h"

#include "iguimanager.h"
#include "guiselector.h"
#include "screenobjectempty.h"

namespace flounder
{
	/// <summary>
	/// A module used for that manages gui textures in a container.
	/// </summary>
	class guis : public imodule
	{
	private:
		iguimanager *m_guiMaster;
		guiselector *m_selector;
		screenobject *m_container;
		std::vector<screenobject*> *m_objects;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static guis *get()
		{
			return static_cast<guis*>(framework::get()->getInstance("guis"));
		}

		/// <summary>
		/// Creates a new guis module.
		/// </summary>
		guis();

		/// <summary>
		/// Deconstructor for the guis module.
		/// </summary>
		~guis();

		/// <summary>
		/// Loads the gui module with inital values.
		/// </summary>
		/// <param name="guimaster"> The inital gui master. </param>
		void load(iguimanager *guimaster);

		void init() override;

		void update() override;

		/// <summary>
		/// Gets the GUI master.
		/// </summary>
		/// <returns> The GUI master. </returns>
		inline iguimanager *getGuiMaster() { return m_guiMaster; };

		/// <summary>
		/// Gets the screen container.
		/// </summary>
		/// <returns> The screen container. </returns>
		inline screenobject *getContainer() { return m_container; }

		/// <summary>
		/// Gets the main GUI selector.
		/// </summary>
		/// <returns> The GUI selector. </returns>
		inline guiselector *getSelector() { return m_selector; }

		/// <summary>
		/// The rendering objects from the container. Updated each update.
		/// </summary>
		/// <returns> The objects. </returns>
		inline std::vector<screenobject*> *getObjects() { return m_objects; };
	};
}

#pragma once

#include "Engine/Engine.hpp"
#include "Fonts/FontFamily.hpp"
#include "UiSelector.hpp"

namespace fl
{
	/// <summary>
	/// A module used for that manages gui textures in a container.
	/// </summary>
	class FL_EXPORT Uis :
		public IModule
	{
	private:
		UiSelector *m_selector;
		UiObject *m_container;
		std::vector<UiObject *> m_objects;
	public:
		std::shared_ptr<FontFamily> m_proximaNova;

		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Uis *Get()
		{
			return reinterpret_cast<Uis *>(Engine::Get()->GetModule("uis"));
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
		std::vector<UiObject *> GetObjects() const { return m_objects; };
	};
}

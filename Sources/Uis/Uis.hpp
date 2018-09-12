#pragma once

#include "Engine/Engine.hpp"
#include "UiSelector.hpp"

namespace acid
{
	/// <summary>
	/// A module used for that manages gui textures in a container.
	/// </summary>
	class ACID_EXPORT Uis :
		public IModule
	{
	private:
		UiSelector m_selector;
		std::unique_ptr<UiObject> m_container;
		std::vector<UiObject *> m_objects;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Uis *Get() { return Engine::Get()->GetModule<Uis>(); }

		Uis();

		void Update() override;

		/// <summary>
		/// Gets the screen container.
		/// </summary>
		/// <returns> The screen container. </returns>
		UiObject *GetContainer() const { return m_container.get(); }

		/// <summary>
		/// Gets the main GUI selector.
		/// </summary>
		/// <returns> The GUI selector. </returns>
		UiSelector &GetSelector() { return m_selector; }

		/// <summary>
		/// The rendering objects from the container. Updated each update.
		/// </summary>
		/// <returns> The objects. </returns>
		std::vector<UiObject *> GetObjects() const { return m_objects; };
	};
}

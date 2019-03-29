#pragma once

#include "Engine/Engine.hpp"
#include "Devices/Mouse.hpp"
#include "UiObject.hpp"

namespace acid
{
/// <summary>
/// A module used for that manages gui textures in a container.
/// </summary>
class ACID_EXPORT Uis :
	public Module
{
public:
	/// <summary>
	/// Gets this engine instance.
	/// </summary>
	/// <returns> The current module instance. </returns>
	static Uis *Get() { return Engine::Get()->GetModuleManager().Get<Uis>(); }

	Uis();

	void Update() override;

	void CancelWasEvent(const MouseButton &button);

	bool IsDown(const MouseButton &button);

	bool WasDown(const MouseButton &button);

	/// <summary>
	/// Gets the screen container.
	/// </summary>
	/// <returns> The screen container. </returns>
	UiObject &GetContainer() { return m_container; }

	/// <summary>
	/// The rendering objects from the container. Updated each update.
	/// </summary>
	/// <returns> The objects. </returns>
	const std::vector<UiObject *> &GetObjects() const { return m_objects; };
private:
	struct SelectorMouse
	{
		bool m_isDown;
		bool m_wasDown;
	};

	std::map<MouseButton, SelectorMouse> m_selectors;
	UiObject m_container;
	std::vector<UiObject *> m_objects;
};
}

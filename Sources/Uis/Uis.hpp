#pragma once

#include "Engine/Engine.hpp"
#include "Devices/Mouse.hpp"
#include "UiObject.hpp"

namespace acid
{
/**
 * @brief Module used for that manages gui textures in a container.
 */
class ACID_EXPORT Uis :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Uis *Get() { return Engine::Get()->GetModule<Uis>(); }

	Uis();

	void Update() override;

	void CancelWasEvent(const MouseButton &button);

	bool IsDown(const MouseButton &button);

	bool WasDown(const MouseButton &button);

	/**
	 * Gets the screen container.
	 * @return The screen container.
	 */
	UiObject &GetContainer() { return m_container; }

	/**
	 * The rendering objects from the container.
	 * @return The objects.
	 */
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

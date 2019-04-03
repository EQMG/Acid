#pragma once

#include "Helpers/Delegate.hpp"
#include "Devices/Mouse.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Transform.hpp"
#include "Maths/Visual/IDriver.hpp"
#include "UiBound.hpp"

namespace acid
{
/**
 * @brief A representation of a object this is rendered to a screen. This object is contained in a parent and has children.
 * The screen object has a few values that allow for it to be positioned and scaled, along with other variables that are used when rendering.
 * This class can be extended to create a representation for GUI textures, fonts, etc.
 */
class ACID_EXPORT UiObject
{
public:
	/**
	 * Creates a new ui object.
	 * @param parent The parent screen object.
	 * @param rectangle The rectangle that will represent the bounds of the ui object.
	 */
	UiObject(UiObject *parent, const UiBound &rectangle);

	virtual ~UiObject();

	/**
	 * Updates this screen object and the extended object.
	 * @param list The list to add to.
	 */
	void Update(std::vector<UiObject *> &list);

	/**
	 * Updates the ui object.
	 */
	virtual void UpdateObject();

	UiObject *GetParent() const { return m_parent; }

	/**
	 * Removes this object from the previous parent and attaches it to another parent.
	 * @param parent The new parent object.
	 */
	void SetParent(UiObject *parent);

	const std::vector<UiObject *> &GetChildren() const { return m_children; }

	/**
	 * Adds a child from this object.
	 * @param child The child to add.
	 */
	void AddChild(UiObject *child);

	/**
	 * Removes a child from this object.
	 * @param child The child to remove.
	 */
	void RemoveChild(UiObject *child);

	void ClearChildren() { m_children.clear(); }

	bool IsEnabled() const;

	void SetEnabled(const bool &enabled) { m_enabled = enabled; }

	UiBound &GetRectangle() { return m_rectangle; }

	void SetRectangle(const UiBound &rectangle) { m_rectangle = rectangle; }

	const Vector4f &GetScissor() const { return m_scissor; }

	void SetScissor(const Vector4f &scissor) { m_scissor = scissor; }

	const float &GetHeight() const { return m_height; }

	void SetHeight(const float &height) { m_height = height; }

	const bool &IsLockRotation() const { return m_lockRotation; }

	void SetLockRotation(const bool &lockRotation) { m_lockRotation = lockRotation; }

	/**
	 * Gets the world transform applied to the object, if has value.
	 * @return The world transform.
	 */
	const std::optional<Transform> &GetWorldTransform() const { return m_worldTransform; }

	/**
	 * Sets the world transform applied to the object.
	 * @param transform The new world space transform.
	 */
	void SetWorldTransform(const std::optional<Transform> &transform) { m_worldTransform = transform; }

	Matrix4 GetModelMatrix() const;

	IDriver<float> *GetAlphaDriver() const { return m_alphaDriver.get(); }

	void SetAlphaDriver(IDriver<float> *alphaDriver) { m_alphaDriver.reset(alphaDriver); }

	const float &GetAlpha() const { return m_alpha; }

	IDriver<float> *GetScaleDriver() const { return m_scaleDriver.get(); }

	void SetScaleDriver(IDriver<float> *scaleDriver) { m_scaleDriver.reset(scaleDriver); }

	const float &GetScale() const { return m_scale; }

	const Vector2f &GetScreenPosition() const { return m_screenPosition; }

	const Vector2f &GetScreenSize() const { return m_screenSize; }

	const float &GetScreenDepth() const { return m_screenDepth; }

	const float &GetScreenAlpha() const { return m_screenAlpha; }

	const float &GetScreenScale() const { return m_screenScale; }
	
	/**
	 * Gets if the object provided has the cursor hovered above it.
	 * @return If the object has the cursor inside of its bounds.
	 */
	bool IsSelected() const { return m_selected; }

	/**
	 * Called when this object has been clicked on.
	 * @return The delegate.
	 */
	Delegate<void(MouseButton)> &OnClick() { return m_onClick; }

	/**
	 * Called when this object has has the cursor hovered over, or removed.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnSelected() { return m_onSelected; }

	void CancelEvent(const MouseButton &button) const;

private:
	UiObject *m_parent;
	std::vector<UiObject *> m_children;

	bool m_enabled;
	UiBound m_rectangle;
	Vector4f m_scissor; // TODO: Convert to UiBound.
	float m_height;

	bool m_lockRotation;
	std::optional<Transform> m_worldTransform;

	std::unique_ptr<IDriver<float>> m_alphaDriver;
	float m_alpha;

	std::unique_ptr<IDriver<float>> m_scaleDriver;
	float m_scale;

	Vector2f m_screenPosition;
	Vector2f m_screenSize;
	float m_screenDepth;
	float m_screenAlpha;
	float m_screenScale;
	bool m_selected;

	Delegate<void(MouseButton)> m_onClick;
	Delegate<void(bool)> m_onSelected;
};
}

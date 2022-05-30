#pragma once

#include "Devices/Mouse.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Transform.hpp"
#include "Constraints/UiConstraints.hpp"
#include "Drivers/UiDriver.hpp"

namespace acid {
/**
 * @brief A representation of a object this is rendered to a screen. This object is contained in a parent and has children.
 * The screen object has a few values that allow for it to be positioned and scaled, along with other variables that are used when rendering.
 * This class can be extended to create a representation for GUI images, fonts, etc.
 */
class ACID_EXPORT UiObject : public virtual rocket::trackable {
public:
	UiObject();
	virtual ~UiObject();

	/**
	 * Updates this screen object and the extended object.
	 * @param viewMatrix The screens orthographic view matrix.
	 * @param list The list to add to.
	 * @param cursorSelect The currently selected UI that has a cursor hover.
	 */
	void Update(const Matrix4 &viewMatrix, std::vector<UiObject *> &list, UiObject *&cursorSelect);

	/**
	 * Updates the ui object.
	 */
	virtual void UpdateObject();

	void CancelEvent(MouseButton button) const;

	const std::vector<UiObject *> &GetChildren() const { return children; }

	/**
	 * Adds a child from this object.
	 * @param child The child to add.
	 */
	virtual void AddChild(UiObject *child);

	/**
	 * Removes a child from this object.
	 * @param child The child to remove.
	 */
	virtual void RemoveChild(UiObject *child);

	void ClearChildren();

	UiObject *GetParent() const { return parent; }

	/**
	 * Removes this object from the previous parent and attaches it to another parent.
	 * @param parent The new parent object.
	 */
	void SetParent(UiObject *parent);

	bool IsEnabled() const;
	void SetEnabled(bool enabled) { this->enabled = enabled; }

	const std::optional<CursorStandard> &GetCursorHover() const { return cursorHover; }
	void SetCursorHover(const std::optional<CursorStandard> &cursorHover) { this->cursorHover = cursorHover; }

	const std::optional<Vector4i> &GetScissor() const { return scissor; }
	void SetScissor(const std::optional<Vector4i> &scissor) { this->scissor = scissor; }

	UiDriver<float> *GetAlphaDriver() const { return alphaDriver.get(); }
	template<template<typename> typename T, typename... Args>
	auto SetAlphaDriver(Args &&... args) -> std::enable_if_t<std::is_convertible_v<T<float> *, UiDriver<float> *>, void> {
		alphaDriver = std::make_unique<T<float>>(std::forward<Args>(args)...);
	}

	UiDriver<Vector2f> *GetScaleDriver() const { return scaleDriver.get(); }
	template<template<typename> typename T, typename... Args>
	auto SetScaleDriver(Args &&... args) -> std::enable_if_t<std::is_convertible_v<T<Vector2f> *, UiDriver<Vector2f> *>, void> {
		scaleDriver = std::make_unique<T<Vector2f>>(std::forward<Args>(args)...);
	}

	UiConstraints &GetConstraints() { return constraints; }
	const UiConstraints &GetConstraints() const { return constraints; }

	const Matrix4 &GetModelView() const { return modelView; }
	const Vector2i &GetScreenPosition() const { return screenPosition; }
	const Vector2i &GetScreenSize() const { return screenSize; }
	float GetScreenDepth() const { return screenDepth; }
	float GetScreenAlpha() const { return screenAlpha; }
	const Vector2f &GetScreenScale() const { return screenScale; }

	/**
	 * Gets if the object provided has the cursor hovered above it.
	 * @return If the object has the cursor inside of its bounds.
	 */
	bool IsSelected() const { return selected; }

	/**
	 * Called when this object has been clicked on.
	 * @return The delegate.
	 */
	rocket::signal<void(MouseButton)> &OnClick() { return onClick; }

	/**
	 * Called when this object has has the cursor hovered over, or removed.
	 * @return The delegate.
	 */
	rocket::signal<void(bool)> &OnSelected() { return onSelected; }

private:
	std::vector<UiObject *> children;
	UiObject *parent = nullptr;

	bool enabled = true;
	std::optional<CursorStandard> cursorHover;
	std::optional<Vector4i> scissor;

	std::unique_ptr<UiDriver<float>> alphaDriver;
	std::unique_ptr<UiDriver<Vector2f>> scaleDriver;

	UiConstraints constraints;
	
	Matrix4 modelView;
	Vector2i screenPosition, screenSize;
	float screenDepth;
	float screenAlpha;
	Vector2f screenScale;
	bool selected = false;

	rocket::signal<void(MouseButton)> onClick;
	rocket::signal<void(bool)> onSelected;
};
}

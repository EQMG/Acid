#pragma once

#include <functional>
#include <optional>
#include <memory>
#include <vector>
#include "Inputs/Mouse.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Transform.hpp"
#include "Maths/Visual/IDriver.hpp"
#include "UiBound.hpp"

namespace acid
{
	/// <summary>
	/// A representation of a object this is rendered to a screen. This object is contained in a parent and has children.
	/// The screen object has a few values that allow for it to be positioned and scaled, along with other variables that are used when rendering.
	/// This class can be extended to create a representation for GUI textures, fonts, etc.
	/// </summary>
	class ACID_EXPORT UiObject
	{
	private:
		UiObject *m_parent;
		std::vector<std::unique_ptr<UiObject>> m_children;

		bool m_visible;
		UiBound m_rectangle;
		Vector4 m_scissor; // TODO: Convert to UiBound.

		Vector2 m_positionOffset;
		Vector4 m_screenTransform;

		bool m_lockRotation;
		std::optional<Transform> m_worldTransform;

		std::unique_ptr<IDriver> m_alphaDriver;
		float m_alpha;

		std::unique_ptr<IDriver> m_scaleDriver;
		float m_scale;

		std::function<bool(MouseButton)> m_actionClick;
	public:
		/// <summary>
		/// Creates a new screen object.
		/// </summary>
		/// <param name="parent"> The parent screen object. </param>
		/// <param name="rectangle"> The rectangle that will represent the bounds of the ui object. </param>
		UiObject(UiObject *parent, const UiBound &rectangle);

		~UiObject();

		/// <summary>
		/// Updates this screen object and the extended object.
		/// </summary>
		/// <param name="list"> The list to add to. </param>
		void Update(std::vector<UiObject *> &list);

		/// <summary>
		/// Updates the implementation.
		/// </summary>
		virtual void UpdateObject();

		/// <summary>
		/// Gets the parent object.
		/// </summary>
		/// <returns> The parent object. </returns>
		UiObject *GetParent() const { return m_parent; }

		/// <summary>
		/// Removes this object from the previous parent and attaches it to another parent.
		/// </summary>
		/// <param name="parent"> The new parent object. </param>
		void SetParent(UiObject *parent);

		const std::vector<std::unique_ptr<UiObject>> &GetChildren() const { return m_children; }

		/// <summary>
		/// Adds a child to this objects children.
		/// </summary>
		/// <param name="child"> The child to add. </param>
		void AddChild(UiObject *child);

		/// <summary>
		/// Disowns a child from this objects children.
		/// </summary>
		/// <param name="child"> The child to disown. </param>
		/// <returns> If the child was disowned. </returns>
		bool RemoveChild(UiObject *child);

		bool IsVisible() const;

		void SetVisible(const bool &visible) { m_visible = visible; }

		UiBound &GetRectangle() { return m_rectangle; }

		void SetRectangle(const UiBound &rectangle) { m_rectangle = rectangle; }

		Vector4 GetScissor() const { return m_scissor; }

		void SetScissor(const Vector4 &scissor) { m_scissor = scissor; }

		Vector2 GetPositionOffset() const { return m_positionOffset; }

		void SetPositionOffset(const Vector2 &positionOffset) { m_positionOffset = positionOffset; }

		/// <summary>
		/// Gets the ui object screen space transform.
		/// </summary>
		/// <returns> The screen transform. </returns>
		Vector4 GetScreenTransform() const { return m_screenTransform; }

		bool IsLockRotation() const { return m_lockRotation; }

		void SetLockRotation(const bool &lockRotation) { m_lockRotation = lockRotation; }

		/// <summary>
		/// Gets the world transform applied to the object, if has value.
		/// </summary>
		/// <returns> The world transform. </returns>
		std::optional<Transform> GetWorldTransform() const { return m_worldTransform; }

		/// <summary>
		/// Sets the world transform applied to the object.
		/// </summary>
		/// <param name="transform"> The new world space transform. </param>
		void SetWorldTransform(const std::optional<Transform> &transform) { m_worldTransform = transform; }

		Matrix4 GetModelMatrix() const;

		IDriver *GetAlphaDriver() const { return m_alphaDriver.get(); }

		/// <summary>
		/// Sets the alpha driver.
		/// </summary>
		/// <param name="alphaDriver"> The new alpha driver. </param>
		void SetAlphaDriver(IDriver *alphaDriver) { m_alphaDriver.reset(alphaDriver); }

		/// <summary>
		/// Sets a new alpha driver from a type.
		/// </summary>
		/// <param name="T"> The type of driver to set. </param>
		/// <param name="args"> The type driver arguments. </param>
		template<typename T, typename... Args>
		void SetAlphaDriver(Args &&... args) { SetAlphaDriver(new T(std::forward<Args>(args)...)); }

		float GetAlpha() const;

		IDriver *GetScaleDriver() const { return m_scaleDriver.get(); }

		/// <summary>
		/// Sets the scale driver.
		/// </summary>
		/// <param name="scaleDriver"> The new scale driver. </param>
		void SetScaleDriver(IDriver *scaleDriver) { m_scaleDriver.reset(scaleDriver); }

		/// <summary>
		/// Sets a new scale driver from a type.
		/// </summary>
		/// <param name="T"> The type of driver to set. </param>
		/// <param name="args"> The type driver arguments. </param>
		template<typename T, typename... Args>
		void SetScaleDriver(Args &&... args) { SetScaleDriver(new T(std::forward<Args>(args)...)); }

		float GetScale() const { return m_scale; }

		void SetActionClick(const std::function<bool(MouseButton)> &actionClick) { m_actionClick = actionClick; }
	};
}

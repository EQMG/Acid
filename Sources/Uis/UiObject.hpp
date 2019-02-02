#pragma once

#include <functional>
#include <optional>
#include <memory>
#include <vector>
#include "Helpers/NonCopyable.hpp"
#include "Helpers/Delegate.hpp"
#include "Devices/Mouse.hpp"
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
	class ACID_EXPORT UiObject :
		public NonCopyable
	{
	public:
		/// <summary>
		/// Creates a new screen object.
		/// </summary>
		/// <param name="parent"> The parent screen object. </param>
		/// <param name="rectangle"> The rectangle that will represent the bounds of the ui object. </param>
		UiObject(UiObject *parent, const UiBound &rectangle);

		virtual ~UiObject();

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
		/// Gets if the object provided has the cursor hovered above it.
		/// </summary>
		/// <param name="object"> The object to check with.
		/// </param>
		/// <returns> If the object has the cursor inside of its box. </returns>
		bool IsSelected() const;

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

		const std::vector<UiObject *> &GetChildren() const { return m_children; }

		/// <summary>
		/// Adds a child to this objects children.
		/// </summary>
		/// <param name="child"> The child to add. </param>
		void AddChild(UiObject *child);

		/// <summary>
		/// Disowns a child from this objects children.
		/// </summary>
		/// <param name="child"> The child to disown. </param>
		void RemoveChild(UiObject *child);

		void ClearChildren() { m_children.clear(); }

		bool IsEnabled() const;

		void SetEnabled(const bool &enabled) { m_enabled = enabled; }

		UiBound &GetRectangle() { return m_rectangle; }

		void SetRectangle(const UiBound &rectangle) { m_rectangle = rectangle; }

		const Vector4 &GetScissor() const { return m_scissor; }

		void SetScissor(const Vector4 &scissor) { m_scissor = scissor; }

		const float &GetHeight() const { return m_height; }

		void SetHeight(const float &height) { m_height = height; }

		const bool &IsLockRotation() const { return m_lockRotation; }

		void SetLockRotation(const bool &lockRotation) { m_lockRotation = lockRotation; }

		/// <summary>
		/// Gets the world transform applied to the object, if has value.
		/// </summary>
		/// <returns> The world transform. </returns>
		const std::optional<Transform> &GetWorldTransform() const { return m_worldTransform; }

		/// <summary>
		/// Sets the world transform applied to the object.
		/// </summary>
		/// <param name="transform"> The new world space transform. </param>
		void SetWorldTransform(const std::optional<Transform> &transform) { m_worldTransform = transform; }

		Matrix4 GetModelMatrix() const;

		IDriver<float> *GetAlphaDriver() const { return m_alphaDriver.get(); }

		/// <summary>
		/// Sets the alpha driver.
		/// </summary>
		/// <param name="alphaDriver"> The new alpha driver. </param>
		void SetAlphaDriver(IDriver<float> *alphaDriver) { m_alphaDriver.reset(alphaDriver); }

		/// <summary>
		/// Sets a new alpha driver from a type.
		/// </summary>
		/// <param name="T"> The type of driver to set. </param>
		/// <param name="args"> The type driver arguments. </param>
		template<typename T, typename... Args>
		void SetAlphaDriver(Args &&... args) { SetAlphaDriver(new T(std::forward<Args>(args)...)); }

		const float &GetAlpha() const { return m_alpha; }

		IDriver<float> *GetScaleDriver() const { return m_scaleDriver.get(); }

		/// <summary>
		/// Sets the scale driver.
		/// </summary>
		/// <param name="scaleDriver"> The new scale driver. </param>
		void SetScaleDriver(IDriver<float> *scaleDriver) { m_scaleDriver.reset(scaleDriver); }

		/// <summary>
		/// Sets a new scale driver from a type.
		/// </summary>
		/// <param name="T"> The type of driver to set. </param>
		/// <param name="args"> The type driver arguments. </param>
		template<typename T, typename... Args>
		void SetScaleDriver(Args &&... args) { SetScaleDriver(new T(std::forward<Args>(args)...)); }

		const float &GetScale() const { return m_scale; }

		const Vector2 &GetScreenDimensions() const { return m_screenDimensions; }

		const Vector2 &GetScreenPosition() const { return m_screenPosition; }

		const float &GetScreenDepth() const { return m_screenDepth; }

		const float &GetScreenAlpha() const { return m_screenAlpha; }

		const float &GetScreenScale() const { return m_screenScale; }

		Delegate<void(UiObject *, MouseButton)> &GetOnClick() { return m_onClick; }

		void CancelEvent(const MouseButton &button) const;
	private:
		UiObject *m_parent;
		std::vector<UiObject *> m_children;

		bool m_enabled;
		UiBound m_rectangle;
		Vector4 m_scissor; // TODO: Convert to UiBound.
		float m_height;

		bool m_lockRotation;
		std::optional<Transform> m_worldTransform;

		std::unique_ptr<IDriver<float>> m_alphaDriver;
		float m_alpha;

		std::unique_ptr<IDriver<float>> m_scaleDriver;
		float m_scale;

		Vector2 m_screenDimensions;
		Vector2 m_screenPosition;
		float m_screenDepth;
		float m_screenAlpha;
		float m_screenScale;

		Delegate<void(UiObject *, MouseButton)> m_onClick;
	};
}

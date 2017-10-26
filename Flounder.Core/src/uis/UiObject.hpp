#pragma once

#include <vector>
#include "../maths/Vector2.hpp"
#include "../maths/Vector4.hpp"
#include "../visual/IDriver.hpp"

namespace Flounder
{
	/// <summary>
	/// A representation of a object this is rendered to a screen. This object is contained in a parent and has children.
	/// The screen object has a few values that allow for it to be positioned and scaled, along with other variables that are used when rendering.
	/// This class can be extended to create a representation for GUI textures, fonts, etc.
	/// </summary>
	class UiObject
	{
	private:
		UiObject *m_parent;
		std::vector<UiObject*> *m_children;

		bool m_visible;
		Vector4 *m_scissor;
		Vector2 *m_position;
		Vector2 *m_dimensions;

		bool m_inScreenCoords;

		Vector2 *m_screenPosition;
		Vector2 *m_screenDimensions;
		Vector2 *m_positionOffsets;

		IDriver *m_alphaDriver;
		float m_alpha;

		IDriver *m_scaleDriver;
		float m_scale;
	public:
		/// <summary>
		/// Creates a new screen object.
		/// </summary>
		/// <param name="parent"> The parent screen object. </param>
		/// <param name="position"> The position in relative space (can be changed to screen space be changing {@code #inScreenCoords} to true.) </param>
		/// <param name="dimensions"> The dimensions of the object, its width is scaled with the aspect ratio so it remains in proportion to the original values. </param>
		UiObject(UiObject *parent, const Vector2 &position, const Vector2 &dimensions);

		/// <summary>
		/// Deconstructor for the screen object.
		/// </summary>
		virtual ~UiObject();

		/// <summary>
		/// Updates this screen object and the extended object.
		/// </summary>
		void Update();

		/// <summary>
		/// Updates the implementation.
		/// </summary>
		virtual void UpdateObject() = 0;

		/// <summary>
		/// Disowns a child from this screen objects children list.
		/// </summary>
		/// <param name="child"> The child to disown. </param>
		void RemoveChild(UiObject *child);

		/// <summary>
		/// Adds this object and its children to a list.
		/// </summary>
		/// <param name="list"> The list to add to.
		/// </param>
		/// <returns> The list that has been added to. </returns>
		std::vector<UiObject*> *GetAll(std::vector<UiObject*> *list);

		/// <summary>
		/// Gets the positions relative in screen space.
		/// </summary>
		/// <returns> The screen positions. </returns>
		Vector2 *GetScreenPosition() const { return m_screenPosition; }

		/// <summary>
		/// Gets the dimensions relative in screen space.
		/// </summary>
		/// <returns> The screen dimensions. </returns>
		Vector2 *GetScreenDimensions() const { return m_screenDimensions; }

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

		std::vector<UiObject*> *GetChildren() const { return m_children; }

		bool IsVisible() const;

		void SetVisible(const bool &visible) { m_visible = visible; }

		Vector4 *GetScissor() const { return m_scissor; }

		void SetScissor(const Vector4 &scissor) const { m_scissor->Set(scissor); }

		Vector2 *GetPosition() const { return m_position; }

		void SetPosition(const Vector2 &position) const { m_position->Set(position); }

		Vector2 *GetDimensions() const { return m_dimensions; }

		void SetDimensions(const Vector2 &dimensions) const { m_dimensions->Set(dimensions); }

		bool GetInScreenCoords() const { return m_inScreenCoords; }

		void SetInScreenCoords(const bool &inScreenCoords) { m_inScreenCoords = inScreenCoords; }

		Vector2 *GetPositionOffsets() const { return m_positionOffsets; }

		void SetPositionOffsets(const Vector2 &positionOffsets) const { m_positionOffsets->Set(positionOffsets); }

		void SetAlphaDriver(IDriver *alphaDriver);

		float GetAlpha() const;

		void SetScaleDriver(IDriver *scaleDriver);

		float GetScale() const { return m_scale; }
	};
}

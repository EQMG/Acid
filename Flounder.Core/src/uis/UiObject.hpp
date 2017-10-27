#pragma once

#include <vector>
#include "../maths/Vector2.hpp"
#include "../maths/Vector3.hpp"
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
		Vector3 *m_dimensions;
		Vector3 *m_position;
		Vector2 *m_pivot;

		Vector4 *m_screenTransform;

		Vector4 *m_scissor;

		IDriver *m_alphaDriver;
		float m_alpha;

		IDriver *m_scaleDriver;
		float m_scale;
	public:
		/// <summary>
		/// Creates a new screen object.
		/// </summary>
		/// <param name="parent"> The parent screen object. </param>
		/// <param name="position"> The position in relative space (if Z is 1 the position will be in screen space, -1 disables screen space). </param>
		/// <param name="dimensions"> The dimensions of the object (if Z is 1 the width will be in screen space, -1 disables screen space). </param>
		/// <param name="pivot"> The pivot vector, this is the bound where the object will be rotated around. Left-Top: (0.0, 0.0), Centre: (0.5, 0.5), Right-Bottom: (1.0, 1.0). </param>
		/// <param name="inScreenCoords"> If all X dimensions and scales will be taken in tems of the screens aspect ratio. </param>
		UiObject(UiObject *parent, const Vector3 &position, const Vector3 &dimensions, const Vector2 &pivot); 

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
		virtual void UpdateObject();

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
		/// Gets the ui object screen space transform.
		/// </summary>
		/// <returns> The screen transform. </returns>
		Vector4 *GetScreenTransform() const { return m_screenTransform; }

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

		Vector3 *GetPosition() const { return m_position; }

		void SetPosition(const Vector2 &position) const { m_position->Set(position); }

		Vector3 *GetDimensions() const { return m_dimensions; }

		void SetDimensions(const Vector2 &dimensions) const { m_dimensions->Set(dimensions); }

		Vector2 *GetPivot() const { return m_pivot; }

		void SetPivot(const Vector2 &pivot) const { m_pivot->Set(pivot); }

		void SetAlphaDriver(IDriver *alphaDriver);

		float GetAlpha() const;

		void SetScaleDriver(IDriver *scaleDriver);

		float GetScale() const { return m_scale; }
	};
}

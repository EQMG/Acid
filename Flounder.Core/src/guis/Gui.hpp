#pragma once

#include "../maths/colour.hpp"
#include "../maths/Vector2.hpp"
#include "../textures/texture.hpp"
#include "../uis/uiobject.hpp"

namespace Flounder
{
	/// <summary>
	/// A object the represents a texture in a GUI.
	/// </summary>
	class gui :
		public UiObject
	{
	private:
		texture *m_texture;
		bool m_flipTexture;
		int m_selectedRow;

		Vector2 *m_textureOffset;
		Colour *m_colourOffset;
	public:
		/// <summary>
		/// Creates a new GUI object.
		/// </summary>
		/// <param name="parent"> The objects parent. </param>
		/// <param name="position"> The objects position relative to the parents. </param>
		/// <param name="dimensions"> The objects dimensions. </param>
		/// <param name="texture"> The objects texture. </param>
		/// <param name="selectedRow"> The default row of the texture to render from. </param>
		gui(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, texture *texture, const int &selectedRow);

		/// <summary>
		/// Deconstructor for the gui object.
		/// </summary>
		~gui();

		void UpdateObject() override;

		texture *getTexture() { return m_texture; }

		void setTexture(texture *texture) { m_texture = texture; }

		bool getFlipTexture() { return m_flipTexture; }

		void setFlipTexture(const bool &flipTexture) { m_flipTexture = flipTexture; }

		int getSelectedRow() { return m_selectedRow; }

		void setSelectedRow(const int &selectedRow) { m_selectedRow = selectedRow; }

		Vector2 *getTextureOffset() { return m_textureOffset; }

		Colour *getColourOffset() { return m_colourOffset; }

		void setColourOffset(const Colour &colourOffset) { m_colourOffset->Set(colourOffset); }
	};
}

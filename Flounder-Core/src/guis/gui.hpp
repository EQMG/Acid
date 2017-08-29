#pragma once

#include "../maths/colour.hpp"
#include "../maths/vector2.hpp"
#include "../textures/texture.hpp"
#include "../uis/uiobject.hpp"

namespace flounder
{
	/// <summary>
	/// A object the represents a texture in a GUI.
	/// </summary>
	class gui :
		public uiobject
	{
	private:
		texture *m_texture;
		bool m_flipTexture;
		int m_selectedRow;

		vector2 *m_textureOffset;
		colour *m_colourOffset;
	public:
		/// <summary>
		/// Creates a new GUI object.
		/// </summary>
		/// <param name="parent"> The objects parent. </param>
		/// <param name="position"> The objects position relative to the parents. </param>
		/// <param name="dimensions"> The objects dimensions. </param>
		/// <param name="texture"> The objects texture. </param>
		/// <param name="selectedRow"> The default row of the texture to render from. </param>
		gui(uiobject *parent, const vector2 &position, const vector2 &dimensions, texture *texture, const int &selectedRow);

		/// <summary>
		/// Deconstructor for the gui object.
		/// </summary>
		~gui();

		void updateObject() override;

		texture *getTexture() { return m_texture; }

		void setTexture(texture *texture) { m_texture = texture; }

		bool getFlipTexture() { return m_flipTexture; }

		void setFlipTexture(const bool &flipTexture) { m_flipTexture = flipTexture; }

		int getSelectedRow() { return m_selectedRow; }

		void setSelectedRow(const int &selectedRow) { m_selectedRow = selectedRow; }

		vector2 *getTextureOffset() { return m_textureOffset; }

		colour *getColourOffset() { return m_colourOffset; }

		void setColourOffset(const colour &colourOffset) { m_colourOffset->set(colourOffset); }
	};
}

#pragma once

#include "../maths/colour.hpp"
#include "../maths/Vector2.hpp"
#include "../textures/Texture.hpp"
#include "../uis/UiObject.hpp"

namespace Flounder
{
	/// <summary>
	/// A object the represents a texture in a GUI.
	/// </summary>
	class Gui :
		public UiObject
	{
	private:
		Texture *m_texture;
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
		Gui(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, Texture *texture, const int &selectedRow);

		/// <summary>
		/// Deconstructor for the gui object.
		/// </summary>
		~Gui();

		void UpdateObject() override;

		Texture *GetTexture() const { return m_texture; }

		void SetTexture(Texture *texture) { m_texture = texture; }

		bool IsFlipTexture() const { return m_flipTexture; }

		void SetFlipTexture(const bool &flipTexture) { m_flipTexture = flipTexture; }

		int GetSelectedRow() const { return m_selectedRow; }

		void SetSelectedRow(const int &selectedRow) { m_selectedRow = selectedRow; }

		Vector2 *GetTextureOffset() const { return m_textureOffset; }

		Colour *GetColourOffset() const { return m_colourOffset; }

		void SetColourOffset(const Colour &colourOffset) const { m_colourOffset->Set(colourOffset); }
	};
}

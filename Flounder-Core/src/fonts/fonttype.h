#pragma once

#include <string>

#include "text.h"
#include "textloader.h"

namespace flounder
{
	/// <summary>
	/// Represents a font. It holds the font's texture atlas as well as having the ability to create the quad vertices for any text using this font.
	/// </summary>
	class fonttype
	{
	private:
		textloader *m_loader;
	public:
		/// <summary>
		/// Creates a new font and loads up the data about each character from the font file.
		/// </summary>
		/// <param name="textureFile"> The file for the font atlas texture. </param>
		/// <param name="fontFile"> The font file containing information about each character in the texture atlas. </param>
		fonttype(const std::string &textureFile, const std::string &fontFile);

		/// <summary>
		/// Deconstructor for the font type.
		/// </summary>
		~fonttype();

		/// <summary>
		/// Takes in an unloaded text and calculate all of the vertices for the quads on which this text will be rendered.
		/// The vertex positions and texture coords and calculated based on the information from the font file.
		/// Then takes the information about the vertices of all the quads and stores it in OpenGL.
		/// </summary>
		/// <param name="text"> The unloaded text. </param>
		void loadText(text *text);

		/// <summary>
		/// Gets the font texture atlas.
		/// </summary>
		/// <returns> The font texture atlas. </returns>
		texture *getTexture();

		/// <summary>
		/// Gets the max line height for this font type.
		/// </summary>
		/// <returns> The max line height. </returns>
		double getMaxSizeY();
	};
}

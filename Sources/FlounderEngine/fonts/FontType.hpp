#pragma once

#include "../Textures/Texture.hpp"
#include "Metafile.hpp"

namespace Flounder
{
	/// <summary>
	/// A loader capable of loading font data into a instance of a text mesh.
	/// </summary>
	class FontType
	{
	private:
		Texture *m_texture;
		Metafile *m_metadata;
	public:
		/// <summary>
		/// Creates a new text loader.
		/// </summary>
		/// <param name="textureFile"> The file for the font atlas texture. </param>
		/// <param name="fontFile"> The font file containing information about each character in the texture atlas. </param>
		FontType(const std::string &textureFile, const std::string &fontFile);

		/// <summary>
		/// Deconstructor for the font type.
		/// </summary>
		~FontType();

		Texture *GetTexture() const { return m_texture; }

		Metafile *GetMetadata() const { return m_metadata; }
	};
}

#pragma once

#include <vector>

#include "../loaders/loaders.h"
#include "../textures/texture.h"
#include "../maths/vector2.h"

#include "metafile.h"
#include "../guis/guialign.h"
#include "line.h"
#include "word.h"

namespace flounder
{
	/// <summary>
	/// A loader capable of loading font data into a instance of a text mesh.
	/// </summary>
	class fonttype
	{
	private:
		texture *m_texture;
		metafile *m_metadata;
	public:
		/// <summary>
		/// Creates a new text loader.
		/// </summary>
		/// <param name="textureFile"> The file for the font atlas texture. </param>
		/// <param name="fontFile"> The font file containing information about each character in the texture atlas. </param>
		fonttype(const std::string &textureFile, const std::string &fontFile);

		/// <summary>
		/// Deconstructor for the font type.
		/// </summary>
		~fonttype();

		texture *getTexture() const { return m_texture; }

		metafile *getMetadata() const { return m_metadata; }

		double getMaxSizeY() { m_metadata->getMaxSizeY(); }
	};
}

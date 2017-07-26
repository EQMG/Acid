#pragma once

#include <vector>

#include "../loaders/loaders.h"
#include "../textures/texture.h"
#include "../maths/vector2.h"

#include "metafile.h"
#include "../guis/guialign.h"
#include "text.h"
#include "line.h"
#include "word.h"

namespace flounder
{
	/// <summary>
	/// A loader capable of loading font data into a instance of a text mesh.
	/// </summary>
	class textloader
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
		textloader(const std::string &textureFile, const std::string &fontFile);

		/// <summary>
		/// Deconstructor for the font type.
		/// </summary>
		~textloader();

		/// <summary>
		/// Creates a mesh for the provided text object using the meta data for this font. Then takes the data created for the text mesh and stores it in OpenGL.
		/// </summary>
		/// <param name="object"> The text object to create a mesh for. </param>
		void loadTextMesh(text *object);
	private:
		std::vector<line*> createStructure(text *object);

		void completeStructure(std::vector<line*> &lines, line *currentLine, word *currentWord, text *object);

		void createQuadVertices(text *object, std::vector<line*> lines, std::vector<GLfloat> *vertices, std::vector<GLfloat> *textures);

		void addVerticesForCharacter(const double &cursorX, const double &cursorY, character *character, std::vector<float> *vertices);

		static void addVertices(const double &x, const double &y, const double &maxX, const double &maxY, std::vector<GLfloat> *vertices);

		static void addTextures(const double &x, const double &y, const double &maxX, const double &maxY, std::vector<GLfloat> *textures);

		vector2 getBounding(std::vector<GLfloat> *vertices);
	public:
		texture *getTexture() const { return m_texture; }

		metafile *getMetadata() const { return m_metadata; }
	};
}

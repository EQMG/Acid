#include "textloader.h"

namespace flounder
{
	textloader::textloader(const std::string &textureFile, const std::string &fontFile)
	{
		m_texture = texture::newTexture()->setFile(textureFile)->create();
		m_metadata = new metafile(fontFile);
	}

	textloader::~textloader()
	{
		delete m_texture;
		delete m_metadata;
	}

	void textloader::loadTextMesh(text *object)
	{
		// Create mesh data.
		std::vector<line*> lines = createStructure(object);
		std::vector<GLfloat> *vertices = new std::vector<float>();
		std::vector<GLfloat> *textures = new std::vector<float>();
		createQuadVertices(object, lines, vertices, textures);
		vector2 meshSize = getBounding(vertices);

		// Load mesh data to OpenGL.
		GLuint vaoID = loaders::get()->createVAO();
		loaders::get()->storeDataInVBO(vaoID, *vertices, 0, 2);
		loaders::get()->storeDataInVBO(vaoID, *textures, 1, 2);
		int verticesCount = vertices->size() / 2;
		text->setMeshInfo(vaoID, verticesCount);
		text->setMeshSize(meshSize);

		delete vertices;
		delete textures;
	}

	std::vector<line*> textloader::createStructure(text *object)
	{
		std::vector<char> chars = object->getTextString().c_str();
		std::vector<line*> lines = std::vector<line*>();
		line *currentLine = new line(m_metadata->getSpaceWidth(), object->getMaxLineSize());
		word *currentWord = new word();

		for (char c : chars)
		{
			int ascii = int(c);

			if (ascii == metafile::SPACE_ASCII)
			{
				bool added = currentLine->addWord(currentWord);

				if (!added)
				{
					lines.push_back(currentLine);
					currentLine = new line(m_metadata->getSpaceWidth(), object->getMaxLineSize());
					currentLine->addWord(currentWord);
				}

				currentWord = new word();
				continue;
			}
			else if (ascii == metafile::NEWLINE_ASCII)
			{
				continue;
			}

			character *character = m_metadata->getCharacter(ascii);
			currentWord->addCharacter(character);
		}

		completeStructure(lines, currentLine, currentWord, object);
		return lines;
	}

	void textloader::completeStructure(std::vector<line*> &lines, line *currentLine, word *currentWord, text *object)
	{
		bool added = currentLine->addWord(currentWord);

		if (!added)
		{
			lines.push_back(currentLine);
			currentLine = new line(m_metadata->getSpaceWidth(), object->getMaxLineSize());
			currentLine->addWord(currentWord);
		}

		lines.push_back(currentLine);
	}

	void textloader::createQuadVertices(text *object, std::vector<line*> lines, std::vector<GLfloat> *vertices, std::vector<GLfloat> *textures)
	{
		object->setNumberOfLines(lines.size());
		double cursorX = 0.0;
		double cursorY = 0.0;

		for (line *line : lines)
		{
			switch (object->getTextAlign())
			{
			case LEFT:
				cursorX = 0.0;
				break;
			case CENTRE:
				cursorX = (line->getMaxLength() - line->getCurrentLineLength()) / 2.0;
				break;
			case RIGHT:
				cursorX = line->getMaxLength() - line->getCurrentLineLength();
				break;
			default:
				cursorX = 0.0;
				break;
			}

			for (word *word : *line->getWords())
			{
				for (character *letter : *word->getCharacters())
				{
					addVerticesForCharacter(cursorX, cursorY, letter, vertices);
					addTextures(letter->getTextureCoordX(), letter->getTextureCoordY(), letter->getMaxTextureCoordX(), letter->getMaxTextureCoordY(), textures);
					cursorX += letter->getAdvanceX();
				}

				cursorX += m_metadata->getSpaceWidth();
			}

			cursorX = 0.0;
			cursorY += metafile::LINE_HEIGHT;
		}
	}

	void textloader::addVerticesForCharacter(const double &cursorX, const double &cursorY, character *character, std::vector<float> *vertices)
	{
		double x = cursorX + character->getOffsetX();
		double y = cursorY + character->getOffsetY();
		double maxX = x + character->getSizeX();
		double maxY = y + character->getSizeY();
		addVertices(x, y, maxX, maxY, vertices);
	}

	void textloader::addVertices(const double &x, const double &y, const double &maxX, const double &maxY, std::vector<GLfloat> *vertices)
	{
		vertices->push_back(static_cast<float>(x));
		vertices->push_back(static_cast<float>(y));
		vertices->push_back(static_cast<float>(x));
		vertices->push_back(static_cast<float>(maxY));
		vertices->push_back(static_cast<float>(maxX));
		vertices->push_back(static_cast<float>(maxY));
		vertices->push_back(static_cast<float>(maxX));
		vertices->push_back(static_cast<float>(maxY));
		vertices->push_back(static_cast<float>(maxX));
		vertices->push_back(static_cast<float>(y));
		vertices->push_back(static_cast<float>(x));
		vertices->push_back(static_cast<float>(y));
	}

	void textloader::addTextures(const double &x, const double &y, const double &maxX, const double &maxY, std::vector<GLfloat> *textures)
	{
		textures->push_back(static_cast<float>(x));
		textures->push_back(static_cast<float>(y));
		textures->push_back(static_cast<float>(x));
		textures->push_back(static_cast<float>(maxY));
		textures->push_back(static_cast<float>(maxX));
		textures->push_back(static_cast<float>(maxY));
		textures->push_back(static_cast<float>(maxX));
		textures->push_back(static_cast<float>(maxY));
		textures->push_back(static_cast<float>(maxX));
		textures->push_back(static_cast<float>(y));
		textures->push_back(static_cast<float>(x));
		textures->push_back(static_cast<float>(y));
	}

	vector2 textloader::getBounding(std::vector<GLfloat> *vertices)
	{
		float minX = +INFINITY;
		float minY = +INFINITY;
		float maxX = -INFINITY;
		float maxY = -INFINITY;
		int i = 0;

		for (float v : *vertices)
		{
			if (i == 0)
			{
				if (v < minX)
				{
					minX = v;
				}
				else if (v > maxX)
				{
					maxX = v;
				}

				i++;
			}
			else if (i == 1)
			{
				if (v < minY)
				{
					minY = v;
				}
				else if (v > maxY)
				{
					maxY = v;
				}

				i = 0;
			}
		}

		return vector2((minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
	}
}

#include "text.hpp"

namespace Flounder
{
	text::text(UiObject *parent, const Vector2 &position, const std::string &text, const float &fontSize, fonttype *fonttype, const float &maxLineLength, const uialign &align) :
		UiObject(parent, position, Vector2(1.0f, 1.0f)),
		m_textString(text),
		m_textAlign(align),
		m_newText(""),
		m_model(nullptr),
		m_lineMaxSize(maxLineLength),
		m_numberOfLines(-1),
		m_fonttype(fonttype),
		m_textColour(new Colour(0.0f, 0.0f, 0.0f, 1.0f)),
		m_borderColour(new Colour(1.0f, 1.0f, 1.0f, 1.0f)),
		m_solidBorder(false),
		m_glowBorder(false),
		m_glowDriver(new driverconstant(0.0f)),
		m_glowSize(0.0f),
		m_borderDriver(new driverconstant(0.0f)),
		m_borderSize(0.0f)
	{
		SetMeshSize(Vector2(0.0f, 0.0f));
		SetScaleDriver(new driverconstant(fontSize));
		loadText(this);
	}

	text::~text()
	{
		delete m_model;

		delete m_textColour;
		delete m_borderColour;

		delete m_glowDriver;
		delete m_borderDriver;
	}

	void text::UpdateObject()
	{
		if (isLoaded() && m_newText != "")
		{
			delete m_model;

			m_textString = m_newText;
			loadText(this);
			m_newText = "";
		}

		switch (m_textAlign)
		{
		case LEFT:
			GetPositionOffsets()->set(GetMeshSize()->m_x * GetScreenDimensions()->m_x, 0.0f);
			break;
		case CENTRE:
			GetPositionOffsets()->set(0.0f, 0.0f);
			break;
		case RIGHT:
			GetPositionOffsets()->set(-GetMeshSize()->m_x * GetScreenDimensions()->m_x, 0.0f);
			break;
		}

		m_glowSize = m_glowDriver->update(Engine::Get()->GetDelta());
		m_borderSize = m_borderDriver->update(Engine::Get()->GetDelta());
	}

	void text::loadText(text *object)
	{
		// Create mesh data.
		std::vector<line*> lines = createStructure(object);
		std::vector<float> *vertices = new std::vector<float>();
		std::vector<float> *textures = new std::vector<float>();
		createQuadVertices(object, lines, vertices, textures);
		Vector2 meshSize = getBounding(vertices);

		// Load mesh data to OpenGL.
		model *loaded = new model(nullptr, vertices, textures, nullptr, nullptr);
		object->setModel(loaded);
		object->SetMeshSize(meshSize);

		//	delete vertices;
		//	delete textures;
	}

	std::vector<line*> text::createStructure(text *object)
	{
		std::vector<line*> lines = std::vector<line*>();
		line *currentLine = new line(object->getFontType()->getMetadata()->getSpaceWidth(), object->getMaxLineSize());
		word *currentWord = new word();

		for (auto c : object->getText())
		{
			int ascii = static_cast<int>(c);

			if (ascii == metafile::SPACE_ASCII)
			{
				bool added = currentLine->addWord(currentWord);

				if (!added)
				{
					lines.push_back(currentLine);
					currentLine = new line(object->getFontType()->getMetadata()->getSpaceWidth(), object->getMaxLineSize());
					currentLine->addWord(currentWord);
				}

				currentWord = new word();
				continue;
			}
			if (ascii == metafile::NEWLINE_ASCII)
			{
				continue;
			}

			character *character = object->getFontType()->getMetadata()->getCharacter(ascii);
			currentWord->addCharacter(character);
		}

		completeStructure(lines, currentLine, currentWord, object);
		return lines;
	}

	void text::completeStructure(std::vector<line*> &lines, line *currentLine, word *currentWord, text *object)
	{
		bool added = currentLine->addWord(currentWord);

		if (!added)
		{
			lines.push_back(currentLine);
			currentLine = new line(object->getFontType()->getMetadata()->getSpaceWidth(), object->getMaxLineSize());
			currentLine->addWord(currentWord);
		}

		lines.push_back(currentLine);
	}

	void text::createQuadVertices(text *object, std::vector<line*> lines, std::vector<float> *vertices, std::vector<float> *textures)
	{
		object->setNumberOfLines((int) lines.size());
		double cursorX = 0.0;
		double cursorY = 0.0;

		for (auto line : lines)
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

			for (auto word : *line->getWords())
			{
				for (auto letter : *word->getCharacters())
				{
					addVerticesForCharacter(cursorX, cursorY, letter, vertices);
					addTextures(letter->getTextureCoordX(), letter->getTextureCoordY(), letter->getMaxTextureCoordX(), letter->getMaxTextureCoordY(), textures);
					cursorX += letter->getAdvanceX();
				}

				cursorX += object->getFontType()->getMetadata()->getSpaceWidth();
			}

			cursorX = 0.0;
			cursorY += metafile::LINE_HEIGHT;
		}
	}

	void text::addVerticesForCharacter(const double &cursorX, const double &cursorY, character *character, std::vector<float> *vertices)
	{
		double x = cursorX + character->getOffsetX();
		double y = cursorY + character->getOffsetY();
		double maxX = x + character->getSizeX();
		double maxY = y + character->getSizeY();
		addVertices(x, y, maxX, maxY, vertices);
	}

	void text::addVertices(const double &x, const double &y, const double &maxX, const double &maxY, std::vector<float> *vertices)
	{
		vertices->push_back(static_cast<float>(x));
		vertices->push_back(static_cast<float>(y));
		vertices->push_back(0.0f);
		vertices->push_back(static_cast<float>(x));
		vertices->push_back(static_cast<float>(maxY));
		vertices->push_back(0.0f);
		vertices->push_back(static_cast<float>(maxX));
		vertices->push_back(static_cast<float>(maxY));
		vertices->push_back(0.0f);
		vertices->push_back(static_cast<float>(maxX));
		vertices->push_back(static_cast<float>(maxY));
		vertices->push_back(0.0f);
		vertices->push_back(static_cast<float>(maxX));
		vertices->push_back(static_cast<float>(y));
		vertices->push_back(0.0f);
		vertices->push_back(static_cast<float>(x));
		vertices->push_back(static_cast<float>(y));
		vertices->push_back(0.0f);
	}

	void text::addTextures(const double &x, const double &y, const double &maxX, const double &maxY, std::vector<float> *textures)
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

	Vector2 text::getBounding(std::vector<float> *vertices)
	{
		float minX = +INFINITY;
		float minY = +INFINITY;
		float maxX = -INFINITY;
		float maxY = -INFINITY;
		int i = 0;

		for (auto v : *vertices)
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

				i++;
			}
			else if (i == 2)
			{
				i = 0;
			}
		}

		return Vector2((minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
	}

	void text::setText(const std::string &newText)
	{
		if (m_textString != newText)
		{
			m_newText = newText;
		}
	}

	void text::setBorder(idriver *driver)
	{
		delete m_borderDriver;
		m_borderDriver = driver;
		m_solidBorder = true;
		m_glowBorder = false;
	}

	void text::setGlowing(idriver *driver)
	{
		delete m_glowDriver;
		m_glowDriver = driver;
		m_solidBorder = false;
		m_glowBorder = true;
	}

	void text::removeBorder()
	{
		m_solidBorder = false;
		m_glowBorder = false;
	}

	float text::getTotalBorderSize()
	{
		if (m_solidBorder)
		{
			if (m_borderSize == 0.0f)
			{
				return 0.0f;
			}
			return calculateEdgeStart() + m_borderSize;
		}
		if (m_glowBorder)
		{
			return calculateEdgeStart();
		}
		return 0.0f;
	}

	float text::getGlowSize()
	{
		if (m_solidBorder)
		{
			return calculateAntialiasSize();
		}
		if (m_glowBorder)
		{
			return m_glowSize;
		}
		return 0.0f;
	}

	float text::calculateEdgeStart()
	{
		float size = GetScale();
		return 1.0f / 300.0f * size + 137.0f / 300.0f;
	}

	float text::calculateAntialiasSize()
	{
		float size = GetScale();
		size = (size - 1.0f) / (1.0f + size / 4.0f) + 1.0f;
		return 0.1f / size;
	}

	bool text::isLoaded()
	{
		return !m_textString.empty() && m_model != nullptr;
	}
}

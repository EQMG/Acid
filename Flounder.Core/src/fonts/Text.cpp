#include "Text.hpp"

#include "../engine/Engine.hpp"
#include "../visual/DriverConstant.hpp"

namespace Flounder
{
	Text::Text(UiObject *parent, const Vector2 &position, const std::string &text, const float &fontSize, FontType *fonttype, const float &maxLineLength, const UiAlign &align) :
		UiObject(parent, position, Vector2(1.0f, 1.0f)),
		m_textString(text),
		m_textAlign(align),
		m_newText(""),
		m_model(nullptr),
		m_lineMaxSize(maxLineLength),
		m_numberOfLines(-1),
		m_fonttype(fonttype),
		m_textColour(new Colour(0.0f, 0.0f, 0.0f)),
		m_borderColour(new Colour(1.0f, 1.0f, 1.0f)),
		m_solidBorder(false),
		m_glowBorder(false),
		m_glowDriver(new DriverConstant(0.0f)),
		m_glowSize(0.0f),
		m_borderDriver(new DriverConstant(0.0f)),
		m_borderSize(0.0f)
	{
		SetMeshSize(Vector2(0.0f, 0.0f));
		SetScaleDriver(new DriverConstant(fontSize));
		LoadText(this);
	}

	Text::~Text()
	{
		delete m_model;

		delete m_textColour;
		delete m_borderColour;

		delete m_glowDriver;
		delete m_borderDriver;
	}

	void Text::UpdateObject()
	{
		if (IsLoaded() && m_newText != "")
		{
			delete m_model;

			m_textString = m_newText;
			LoadText(this);
			m_newText = "";
		}

		switch (m_textAlign)
		{
		case AlignLeft:
			GetPositionOffsets()->Set(GetMeshSize()->m_x * GetScreenDimensions()->m_x, 0.0f);
			break;
		case AlightCentre:
			GetPositionOffsets()->Set(0.0f, 0.0f);
			break;
		case CentreRight:
			GetPositionOffsets()->Set(-GetMeshSize()->m_x * GetScreenDimensions()->m_x, 0.0f);
			break;
		}

		m_glowSize = m_glowDriver->Update(Engine::Get()->GetDelta());
		m_borderSize = m_borderDriver->Update(Engine::Get()->GetDelta());
	}

	void Text::LoadText(Text *object)
	{
		// Create mesh data.
		std::vector<Line> lines = CreateStructure(object);
		std::vector<float> vertices = std::vector<float>();
		std::vector<float> textures = std::vector<float>();
		CreateQuadVertices(object, lines, vertices, textures);
		Vector2 meshSize = GetBounding(vertices);

		// Load mesh data to OpenGL. TODO
	//	Model *loaded = new Model(std::vector<int>(), vertices, textures);
	//	object->SetModel(loaded);
	//	object->SetMeshSize(meshSize);
	}

	std::vector<Line> Text::CreateStructure(Text *object)
	{
		std::vector<Line> lines = std::vector<Line>();
		Line currentLine = Line(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->GetMaxLineSize());
		Word currentWord = Word();

		for (auto c : object->GetText())
		{
			int ascii = static_cast<int>(c);

			if (ascii == Metafile::SPACE_ASCII)
			{
				bool added = currentLine.AddWord(currentWord);

				if (!added)
				{
					lines.push_back(currentLine);
					currentLine = Line(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->GetMaxLineSize());
					currentLine.AddWord(currentWord);
				}

				currentWord = Word();
				continue;
			}
			if (ascii == Metafile::NEWLINE_ASCII)
			{
				continue;
			}

			Character *character = object->GetFontType()->GetMetadata()->GetCharacter(ascii);

			if (character != nullptr)
			{
				currentWord.AddCharacter(*character);
			}
		}

		CompleteStructure(lines, currentLine, currentWord, object);
		return lines;
	}

	void Text::CompleteStructure(std::vector<Line> &lines, Line &currentLine, const Word &currentWord, Text *object)
	{
		bool added = currentLine.AddWord(currentWord);

		if (!added)
		{
			lines.push_back(currentLine);
			currentLine = Line(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->GetMaxLineSize());
			currentLine.AddWord(currentWord);
		}

		lines.push_back(currentLine);
	}

	void Text::CreateQuadVertices(Text *object, std::vector<Line> lines, std::vector<float> &vertices, std::vector<float> &textures)
	{
		object->SetNumberOfLines(static_cast<int>(lines.size()));
		double cursorX = 0.0;
		double cursorY = 0.0;

		for (auto line : lines)
		{
			switch (object->GetTextAlign())
			{
			case AlignLeft:
				cursorX = 0.0;
				break;
			case AlightCentre:
				cursorX = (line.GetMaxLength() - line.GetCurrentLineLength()) / 2.0;
				break;
			case CentreRight:
				cursorX = line.GetMaxLength() - line.GetCurrentLineLength();
				break;
			default:
				cursorX = 0.0;
				break;
			}

			for (auto word : line.GetWords())
			{
				for (auto letter : word.GetCharacters())
				{
					AddVerticesForCharacter(cursorX, cursorY, letter, vertices);
					AddTextures(letter.GetTextureCoordX(), letter.GetTextureCoordY(), letter.GetMaxTextureCoordX(), letter.GetMaxTextureCoordY(), textures);
					cursorX += letter.GetAdvanceX();
				}

				cursorX += object->GetFontType()->GetMetadata()->GetSpaceWidth();
			}

			cursorX = 0.0;
			cursorY += Metafile::LINE_HEIGHT;
		}
	}

	void Text::AddVerticesForCharacter(const double &cursorX, const double &cursorY, const Character &character, std::vector<float> &vertices)
	{
		double x = cursorX + character.GetOffsetX();
		double y = cursorY + character.GetOffsetY();
		double maxX = x + character.GetSizeX();
		double maxY = y + character.GetSizeY();
		AddVertices(x, y, maxX, maxY, vertices);
	}

	void Text::AddVertices(const double &x, const double &y, const double &maxX, const double &maxY, std::vector<float> &vertices)
	{
		vertices.push_back(static_cast<float>(x));
		vertices.push_back(static_cast<float>(y));
		vertices.push_back(0.0f);
		vertices.push_back(static_cast<float>(x));
		vertices.push_back(static_cast<float>(maxY));
		vertices.push_back(0.0f);
		vertices.push_back(static_cast<float>(maxX));
		vertices.push_back(static_cast<float>(maxY));
		vertices.push_back(0.0f);
		vertices.push_back(static_cast<float>(maxX));
		vertices.push_back(static_cast<float>(maxY));
		vertices.push_back(0.0f);
		vertices.push_back(static_cast<float>(maxX));
		vertices.push_back(static_cast<float>(y));
		vertices.push_back(0.0f);
		vertices.push_back(static_cast<float>(x));
		vertices.push_back(static_cast<float>(y));
		vertices.push_back(0.0f);
	}

	void Text::AddTextures(const double &x, const double &y, const double &maxX, const double &maxY, std::vector<float> &textures)
	{
		textures.push_back(static_cast<float>(x));
		textures.push_back(static_cast<float>(y));
		textures.push_back(static_cast<float>(x));
		textures.push_back(static_cast<float>(maxY));
		textures.push_back(static_cast<float>(maxX));
		textures.push_back(static_cast<float>(maxY));
		textures.push_back(static_cast<float>(maxX));
		textures.push_back(static_cast<float>(maxY));
		textures.push_back(static_cast<float>(maxX));
		textures.push_back(static_cast<float>(y));
		textures.push_back(static_cast<float>(x));
		textures.push_back(static_cast<float>(y));
	}

	Vector2 Text::GetBounding(std::vector<float> &vertices)
	{
		float minX = +INFINITY;
		float minY = +INFINITY;
		float maxX = -INFINITY;
		float maxY = -INFINITY;
		int i = 0;

		for (auto v : vertices)
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

	void Text::setText(const std::string &newText)
	{
		if (m_textString != newText)
		{
			m_newText = newText;
		}
	}

	void Text::SetBorder(IDriver *driver)
	{
		delete m_borderDriver;
		m_borderDriver = driver;
		m_solidBorder = true;
		m_glowBorder = false;
	}

	void Text::SetGlowing(IDriver *driver)
	{
		delete m_glowDriver;
		m_glowDriver = driver;
		m_solidBorder = false;
		m_glowBorder = true;
	}

	void Text::RemoveBorder()
	{
		m_solidBorder = false;
		m_glowBorder = false;
	}

	float Text::GetTotalBorderSize()
	{
		if (m_solidBorder)
		{
			if (m_borderSize == 0.0f)
			{
				return 0.0f;
			}
			return CalculateEdgeStart() + m_borderSize;
		}
		if (m_glowBorder)
		{
			return CalculateEdgeStart();
		}
		return 0.0f;
	}

	float Text::GetGlowSize()
	{
		if (m_solidBorder)
		{
			return CalculateAntialiasSize();
		}
		if (m_glowBorder)
		{
			return m_glowSize;
		}
		return 0.0f;
	}

	float Text::CalculateEdgeStart()
	{
		float size = GetScale();
		return 1.0f / 300.0f * size + 137.0f / 300.0f;
	}

	float Text::CalculateAntialiasSize()
	{
		float size = GetScale();
		size = (size - 1.0f) / (1.0f + size / 4.0f) + 1.0f;
		return 0.1f / size;
	}

	bool Text::IsLoaded()
	{
		return !m_textString.empty() && m_model != nullptr;
	}
}

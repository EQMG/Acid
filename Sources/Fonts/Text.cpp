#include "Text.hpp"

#include "Maths/Visual/DriverConstant.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	Text::Text(UiObject *parent, const UiBound &rectangle, const float &fontSize, const std::string &text, const std::shared_ptr<FontType> &fontType, const TextJustify &justify, const float &maxWidth, const float &kerning, const float &leading) :
		UiObject(parent, rectangle),
		m_descriptorSet(DescriptorsHandler()),
		m_uniformObject(UniformHandler()),
		m_model(nullptr),
		m_numberLines(0),
		m_string(text),
		m_newString(""),
		m_justify(justify),
		m_fontType(fontType),
		m_maxWidth(maxWidth),
		m_kerning(kerning),
		m_leading(leading),
		m_textColour(Colour("#ffffff")),
		m_borderColour(Colour("#000000")),
		m_solidBorder(false),
		m_glowBorder(false),
		m_glowDriver(std::make_shared<DriverConstant>(0.0f)),
		m_glowSize(0.0f),
		m_borderDriver(std::make_shared<DriverConstant>(0.0f)),
		m_borderSize(0.0f)
	{
		SetScaleDriver(std::make_shared<DriverConstant>(fontSize));
		LoadText();
	}

	Text::~Text()
	{
	}

	void Text::UpdateObject()
	{
		if (IsLoaded() && !m_newString.empty())
		{
			m_string = m_newString;
			LoadText();
			m_newString = "";
		}

		m_glowSize = m_glowDriver->Update(Engine::Get()->GetDelta());
		m_borderSize = m_borderDriver->Update(Engine::Get()->GetDelta());

		// Updates uniforms.
		m_uniformObject.Push("transform", GetScreenTransform());
		m_uniformObject.Push("colour", m_textColour);
		m_uniformObject.Push("borderColour", m_borderColour);
		m_uniformObject.Push("borderSizes", Vector2(GetTotalBorderSize(), GetGlowSize()));
		m_uniformObject.Push("edgeData", Vector2(CalculateEdgeStart(), CalculateAntialiasSize()));
		m_uniformObject.Push("alpha", GetAlpha());
	}

	void Text::CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		// Gets if this should be rendered.
		if (m_model == nullptr || !IsVisible() || GetAlpha() == 0.0f)
		{
			return;
		}

		// Updates descriptors.
		m_descriptorSet.Push("UboObject", m_uniformObject);
		m_descriptorSet.Push("samplerColour", m_fontType->GetTexture());
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return;
		}

		VkRect2D scissorRect = {};
		scissorRect.offset.x = static_cast<uint32_t>(Display::Get()->GetWidth() * GetScissor().m_x);
		scissorRect.offset.y = static_cast<uint32_t>(Display::Get()->GetHeight() * GetScissor().m_y);
		scissorRect.extent.width = static_cast<uint32_t>(Display::Get()->GetWidth() * GetScissor().m_z);
		scissorRect.extent.height = static_cast<uint32_t>(Display::Get()->GetHeight() * GetScissor().m_w);
		vkCmdSetScissor(commandBuffer.GetCommandBuffer(), 0, 1, &scissorRect);

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}

	void Text::SetString(const std::string &newString)
	{
		if (m_string != newString)
		{
			m_newString = newString;
		}
	}

	void Text::SetBorderDriver(const std::shared_ptr<IDriver> &driver)
	{
		m_borderDriver = driver;
		m_solidBorder = true;
		m_glowBorder = false;
	}

	void Text::SetGlowingDriver(const std::shared_ptr<IDriver> &driver)
	{
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
		float size = 0.5f * GetScale();
		return 1.0f / 300.0f * size + 137.0f / 300.0f;
	}

	float Text::CalculateAntialiasSize()
	{
		float size = 0.5f * GetScale();
		size = (size - 1.0f) / (1.0f + size / 4.0f) + 1.0f;
		return 0.1f / size;
	}

	bool Text::IsLoaded()
	{
		return !m_string.empty() && m_model != nullptr;
	}

	void Text::LoadText()
	{
		// Creates mesh data.
		auto lines = CreateStructure();
		auto vertices = CreateQuad(lines);

		// Calculates the bounds and normalizes the vertices.
		Vector2 bounding = Vector2();
		NormalizeQuad(bounding, vertices);

		// Loads the mesh data.
		delete m_model;
		m_model = new Model(vertices);
		GetRectangle().SetDimensions(Vector2(bounding.m_x, bounding.m_y));
	}

	std::vector<FontLine> Text::CreateStructure()
	{
		auto lines = std::vector<FontLine>();
		auto currentLine = FontLine(m_fontType->GetMetadata()->GetSpaceWidth(), m_maxWidth);
		auto currentWord = FontWord();

		auto formattedText = String::Replace(m_string, "\t", "	");
		auto textLines = String::Split(formattedText, "\n", true);

		for (uint32_t i = 0; i < textLines.size(); i++)
		{
			if (textLines.at(i).empty())
			{
				continue;
			}

			for (auto &c : textLines.at(i))
			{
				int ascii = static_cast<int>(c);

				if (ascii == FontMetafile::SPACE_ASCII)
				{
					bool added = currentLine.AddWord(currentWord);

					if (!added)
					{
						lines.emplace_back(currentLine);
						currentLine = FontLine(m_fontType->GetMetadata()->GetSpaceWidth(), m_maxWidth);
						currentLine.AddWord(currentWord);
					}

					currentWord = FontWord();
					continue;
				}

				auto character = m_fontType->GetMetadata()->GetCharacter(ascii);

				if (character)
				{
					currentWord.AddCharacter(*character, m_kerning);
				}
			}

			if (i != textLines.size() - 1)
			{
				lines.emplace_back(currentLine);
				currentLine = FontLine(m_fontType->GetMetadata()->GetSpaceWidth(), m_maxWidth);
				currentLine.AddWord(currentWord);
			}
		}

		CompleteStructure(lines, currentLine, currentWord);
		return lines;
	}

	void Text::CompleteStructure(std::vector<FontLine> &lines, FontLine &currentLine, const FontWord &currentWord)
	{
		bool added = currentLine.AddWord(currentWord);

		if (!added)
		{
			lines.emplace_back(currentLine);
			currentLine = FontLine(m_fontType->GetMetadata()->GetSpaceWidth(), m_maxWidth);
			currentLine.AddWord(currentWord);
		}

		lines.emplace_back(currentLine);
	}

	std::vector<IVertex *> Text::CreateQuad(const std::vector<FontLine> &lines)
	{
		auto vertices = std::vector<IVertex *>();
		m_numberLines = static_cast<uint32_t>(lines.size());

		float cursorX = 0.0f;
		float cursorY = 0.0f;
		int32_t lineOrder = static_cast<int32_t>(lines.size());

		for (auto &line : lines)
		{
			switch (m_justify)
			{
			case JUSTIFY_LEFT:
				cursorX = 0.0f;
				break;
			case JUSTIFY_CENTRE:
				cursorX = (line.GetMaxLength() - line.GetCurrentLineLength()) / 2.0f;
				break;
			case JUSTIFY_RIGHT:
				cursorX = line.GetMaxLength() - line.GetCurrentLineLength();
				break;
			case JUSTIFY_FULLY:
				cursorX = 0.0f;
				break;
			}

			for (auto &word : line.GetWords())
			{
				for (auto &letter : word.GetCharacters())
				{
					AddVerticesForCharacter(cursorX, cursorY, letter, vertices);
					cursorX += m_kerning + letter.GetAdvanceX();
				}

				if (m_justify == JUSTIFY_FULLY && lineOrder > 1)
				{
					cursorX += (line.GetMaxLength() - line.GetCurrentWordsLength()) / line.GetWords().size();
				}
				else
				{
					cursorX += m_fontType->GetMetadata()->GetSpaceWidth();
				}
			}

			cursorY += m_leading + FontMetafile::LINE_HEIGHT;
			lineOrder--;
		}

		return vertices;
	}

	void Text::AddVerticesForCharacter(const float &cursorX, const float &cursorY, const FontCharacter &character, std::vector<IVertex *> &vertices)
	{
		float vertexX = cursorX + character.GetOffsetX();
		float vertexY = cursorY + character.GetOffsetY();
		float vertexMaxX = vertexX + character.GetSizeX();
		float vertexMaxY = vertexY + character.GetSizeY();

		float textureX = character.GetTextureCoordX();
		float textureY = character.GetTextureCoordY();
		float textureMaxX = character.GetMaxTextureCoordX();
		float textureMaxY = character.GetMaxTextureCoordY();

		AddVertex(vertexX, vertexY, textureX, textureY, vertices);
		AddVertex(vertexX, vertexMaxY, textureX, textureMaxY, vertices);
		AddVertex(vertexMaxX, vertexMaxY, textureMaxX, textureMaxY, vertices);
		AddVertex(vertexMaxX, vertexMaxY, textureMaxX, textureMaxY, vertices);
		AddVertex(vertexMaxX, vertexY, textureMaxX, textureY, vertices);
		AddVertex(vertexX, vertexY, textureX, textureY, vertices);
	}

	void Text::AddVertex(const float &vx, const float &vy, const float &tx, const float &ty, std::vector<IVertex *> &vertices)
	{
		IVertex *vertex = new VertexModel(Vector3(static_cast<float>(vx), static_cast<float>(vy), 0.0f), Vector2(static_cast<float>(tx), static_cast<float>(ty)));
		vertices.emplace_back(vertex);
	}

	void Text::NormalizeQuad(Vector2 &bounding, std::vector<IVertex *> &vertices)
	{
		float minX = +INFINITY;
		float minY = +INFINITY;
		float maxX = -INFINITY;
		float maxY = -INFINITY;

		for (auto &vertex : vertices)
		{
			Vector3 position = vertex->GetPosition();

			if (position.m_x < minX)
			{
				minX = position.m_x;
			}
			else if (position.m_x > maxX)
			{
				maxX = position.m_x;
			}

			if (position.m_y < minY)
			{
				minY = position.m_y;
			}
			else if (position.m_y > maxY)
			{
				maxY = position.m_y;
			}
		}

		if (m_justify == JUSTIFY_CENTRE)
		{
			minX = 0.0f;
			maxX = m_maxWidth;
		}

	//	maxY = static_cast<float>(GetFontType()->GetMetadata()->GetMaxSizeY()) * m_numberLines;
		bounding = Vector2((maxX - minX) / 2.0f, (maxY - minX) / 2.0f);

		for (auto &vertex : vertices)
		{
			Vector3 position = Vector3((vertex->GetPosition().m_x - minX) / (maxX - minX), (vertex->GetPosition().m_y - minY) / (maxY - minY), 0.0f);
			vertex->SetPosition(position);
		}
	}
}

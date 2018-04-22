#include "Text.hpp"

#include "Maths/Visual/DriverConstant.hpp"

namespace Flounder
{
	Text::Text(UiObject *parent, const UiBound &rectangle, const float &fontSize, const std::string &text, FontType *fontType, const FontJustify &justify, const float &maxWidth, const float &kerning, const float &leading) :
		UiObject(parent, rectangle),
		m_descriptorSet(new DescriptorsHandler()),
		m_uniformObject(new UniformHandler()),
		m_model(nullptr),
		m_string(text),
		m_newString(""),
		m_justify(justify),
		m_fontType(fontType),
		m_maxWidth(maxWidth),
		m_kerning(kerning),
		m_leading(leading),
		m_textColour(new Colour("#ffffff")),
		m_borderColour(new Colour("#000000")),
		m_solidBorder(false),
		m_glowBorder(false),
		m_glowDriver(new DriverConstant(0.0f)),
		m_glowSize(0.0f),
		m_borderDriver(new DriverConstant(0.0f)),
		m_borderSize(0.0f)
	{
		SetScaleDriver(new DriverConstant(fontSize));
		LoadText(this);
	}

	Text::~Text()
	{
		delete m_descriptorSet;
		delete m_uniformObject;
		delete m_model;

		delete m_textColour;
		delete m_borderColour;

		delete m_glowDriver;
		delete m_borderDriver;
	}

	void Text::UpdateObject()
	{
		if (IsLoaded() && m_newString != "")
		{
			delete m_model;
			m_string = m_newString;
			LoadText(this);
			m_newString = "";
		}

		m_glowSize = m_glowDriver->Update(Engine::Get()->GetDelta());
		m_borderSize = m_borderDriver->Update(Engine::Get()->GetDelta());

		// Updates uniforms.
		m_uniformObject->Push("transform", *GetScreenTransform());
		m_uniformObject->Push("colour", *m_textColour);
		m_uniformObject->Push("borderColour", *m_borderColour);
		m_uniformObject->Push("borderSizes", Vector2(GetTotalBorderSize(), GetGlowSize()));
		m_uniformObject->Push("edgeData", Vector2(CalculateEdgeStart(), CalculateAntialiasSize()));
		m_uniformObject->Push("alpha", GetAlpha());
	}

	void Text::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		// Gets if this should be rendered.
		if (m_model == nullptr || !IsVisible() || GetAlpha() == 0.0f)
		{
			return;
		}

		// Updates descriptors.
		m_descriptorSet->Push("UboObject", m_uniformObject);
		m_descriptorSet->Push("samplerColour", m_fontType->GetTexture());
		bool descriptorsSet = m_descriptorSet->Update(pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		VkRect2D scissorRect = {};
		scissorRect.offset.x = static_cast<uint32_t>(Display::Get()->GetWidth() * GetScissor()->m_x);
		scissorRect.offset.y = static_cast<uint32_t>(Display::Get()->GetHeight() * GetScissor()->m_y);
		scissorRect.extent.width = static_cast<uint32_t>(Display::Get()->GetWidth() * GetScissor()->m_z);
		scissorRect.extent.height = static_cast<uint32_t>(Display::Get()->GetHeight() * GetScissor()->m_w);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

		// Draws the object.
		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}

	void Text::SetText(const std::string &newText)
	{
		if (m_string != newText)
		{
			m_newString = newText;
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

	void Text::LoadText(Text *object)
	{
		// Creates mesh data.
		std::vector<FontLine> lines = CreateStructure(object);
		std::vector<IVertex *> vertices = CreateQuad(object, lines);

		// Calculates the bounds and normalizes the vertices.
		Vector2 bounding = Vector2();
		NormalizeQuad(&bounding, vertices);

		// Loads mesh data to Vulkan.
		Model *model = new Model(vertices);
		object->m_model = model;
		*object->GetRectangle()->m_dimensions = Vector2(bounding.m_x, bounding.m_y);
	}

	std::vector<FontLine> Text::CreateStructure(Text *object)
	{
		std::vector<FontLine> lines = std::vector<FontLine>();
		FontLine currentLine = FontLine(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->m_maxWidth);
		FontWord currentWord = FontWord();

		std::string formattedText = FormatString::Replace(object->GetText(), "\t", "    ");
		std::vector<std::string> textLines = FormatString::Split(formattedText, "\n", true);

		for (unsigned int i = 0; i < textLines.size(); i++)
		{
			if (textLines.at(i).empty())
			{
				continue;
			}

			for (auto c : textLines.at(i))
			{
				int ascii = static_cast<int>(c);

				if (ascii == FontMetafile::SPACE_ASCII)
				{
					bool added = currentLine.AddWord(currentWord);

					if (!added)
					{
						lines.push_back(currentLine);
						currentLine = FontLine(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->m_maxWidth);
						currentLine.AddWord(currentWord);
					}

					currentWord = FontWord();
					continue;
				}

				FontCharacter *character = object->GetFontType()->GetMetadata()->GetCharacter(ascii);

				if (character != nullptr)
				{
					currentWord.AddCharacter(*character, object->m_kerning);
				}
			}

			if (i != textLines.size() - 1)
			{
				lines.push_back(currentLine);
				currentLine = FontLine(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->m_maxWidth);
				currentLine.AddWord(currentWord);
			}
		}

		CompleteStructure(lines, currentLine, currentWord, object);
		return lines;
	}

	void Text::CompleteStructure(std::vector<FontLine> &lines, FontLine &currentLine, const FontWord &currentWord, Text *object)
	{
		bool added = currentLine.AddWord(currentWord);

		if (!added)
		{
			lines.push_back(currentLine);
			currentLine = FontLine(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->m_maxWidth);
			currentLine.AddWord(currentWord);
		}

		lines.push_back(currentLine);
	}

	std::vector<IVertex *> Text::CreateQuad(Text *object, std::vector<FontLine> lines)
	{
		std::vector<IVertex *> vertices = std::vector<IVertex *>();
		//	object->m_numberLines = static_cast<int>(lines.size());
		double cursorX = 0.0;
		double cursorY = 0.0;
		int lineOrder = static_cast<int>(lines.size());

		for (const auto &line : lines)
		{
			switch (object->GetTextJustify())
			{
			case JustifyLeft:
				cursorX = 0.0;
				break;
			case JustifyCentre:
				cursorX = (line.GetMaxLength() - line.GetCurrentLineLength()) / 2.0;
				break;
			case JustifyRight:
				cursorX = line.GetMaxLength() - line.GetCurrentLineLength();
				break;
			case JustifyFully:
				cursorX = 0.0;
				break;
			}

			for (const auto &word : line.GetWords())
			{
				for (const auto &letter : word.GetCharacters())
				{
					AddVerticesForCharacter(cursorX, cursorY, letter, vertices);
					cursorX += object->m_kerning + letter.GetAdvanceX();
				}

				if (object->GetTextJustify() == JustifyFully && lineOrder > 1)
				{
					cursorX += (line.GetMaxLength() - line.GetCurrentWordsLength()) / line.GetWords().size();
				}
				else
				{
					cursorX += object->GetFontType()->GetMetadata()->GetSpaceWidth();
				}
			}

			cursorX = 0.0;
			cursorY += object->m_leading + FontMetafile::LINE_HEIGHT;
			lineOrder--;
		}

		return vertices;
	}

	void Text::AddVerticesForCharacter(const double &cursorX, const double &cursorY, const FontCharacter &character, std::vector<IVertex *> &vertices)
	{
		const double vertexX = cursorX + character.GetOffsetX();
		const double vertexY = cursorY + character.GetOffsetY();
		const double vertexMaxX = vertexX + character.GetSizeX();
		const double vertexMaxY = vertexY + character.GetSizeY();

		const double textureX = character.GetTextureCoordX();
		const double textureY = character.GetTextureCoordY();
		const double textureMaxX = character.GetMaxTextureCoordX();
		const double textureMaxY = character.GetMaxTextureCoordY();

		AddVertex(vertexX, vertexY, textureX, textureY, vertices);
		AddVertex(vertexX, vertexMaxY, textureX, textureMaxY, vertices);
		AddVertex(vertexMaxX, vertexMaxY, textureMaxX, textureMaxY, vertices);
		AddVertex(vertexMaxX, vertexMaxY, textureMaxX, textureMaxY, vertices);
		AddVertex(vertexMaxX, vertexY, textureMaxX, textureY, vertices);
		AddVertex(vertexX, vertexY, textureX, textureY, vertices);
	}

	void Text::AddVertex(const double &vx, const double &vy, const double &tx, const double &ty, std::vector<IVertex *> &vertices)
	{
		IVertex *vertex = new VertexModel(Vector3(static_cast<float>(vx), static_cast<float>(vy), 0.0f), Vector2(static_cast<float>(tx), static_cast<float>(ty)));
		vertices.push_back(vertex);
	}

	void Text::NormalizeQuad(Vector2 *bounding, std::vector<IVertex *> &vertices)
	{
		float minX = +INFINITY;
		float minY = +INFINITY;
		float maxX = -INFINITY;
		float maxY = -INFINITY;

		for (auto vertex : vertices)
		{
			const Vector3 position = vertex->GetPosition();

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

		// TODO: Ignore min values, they allow better alignment!
		// maxY = static_cast<float>(m_text->GetFontType()->GetMetadata()->GetMaxSizeY()) * object->m_numberLines;
		*bounding = Vector2((maxX - minX) / 2.0f, (maxY - minX) / 2.0f);
		maxX -= minX;
		maxY -= minY;

		for (auto vertex : vertices)
		{
			Vector3 position = Vector3((vertex->GetPosition().m_x - minX) / maxX, (vertex->GetPosition().m_y - minY) / maxY, 0.0f);
			vertex->SetPosition(position);
		}
	}
}

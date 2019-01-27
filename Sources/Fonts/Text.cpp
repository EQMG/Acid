#include "Text.hpp"

#include "Maths/Visual/DriverConstant.hpp"

namespace acid
{
	Text::Text(UiObject *parent, const UiBound &rectangle, const float &fontSize, const std::string &text, const std::shared_ptr<FontType> &fontType,
		const Text::Justify &justify, const float &maxWidth, const Colour &textColour, const float &kerning, const float &leading) :
		UiObject(parent, rectangle),
		m_descriptorSet(DescriptorsHandler()),
		m_uniformObject(UniformHandler()),
		m_model(nullptr),
		m_numberLines(0),
		m_string(text),
		m_newString({}),
		m_justify(justify),
		m_fontType(fontType),
		m_maxWidth(maxWidth),
		m_kerning(kerning),
		m_leading(leading),
		m_textColour(textColour),
		m_borderColour(Colour::White),
		m_solidBorder(false),
		m_glowBorder(false),
		m_glowDriver(std::make_unique<DriverConstant<float>>(0.0f)),
		m_glowSize(0.0f),
		m_borderDriver(std::make_unique<DriverConstant<float>>(0.0f)),
		m_borderSize(0.0f)
	{
		SetScaleDriver<DriverConstant<float>>(fontSize);
		LoadText();
	}

	void Text::UpdateObject()
	{
		if (m_newString.has_value())
		{
			m_string = *m_newString;
			LoadText();
			m_newString = {};
		}

		m_glowSize = m_glowDriver->Update(Engine::Get()->GetDelta());
		m_borderSize = m_borderDriver->Update(Engine::Get()->GetDelta());

		// Updates uniforms.
		m_uniformObject.Push("modelMatrix", GetModelMatrix());
		m_uniformObject.Push("colour", m_textColour);
		m_uniformObject.Push("borderColour", m_borderColour);
		m_uniformObject.Push("borderSizes", Vector2(GetTotalBorderSize(), GetGlowSize()));
		m_uniformObject.Push("edgeData", Vector2(CalculateEdgeStart(), CalculateAntialiasSize()));
		m_uniformObject.Push("screenDimension", 2.0f * GetScreenDimension());
		m_uniformObject.Push("screenPosition", 2.0f * GetScreenPosition() - 1.0f);
		m_uniformObject.Push("alpha", GetAlpha());
		m_uniformObject.Push("depth", GetDepth());
		m_uniformObject.Push("modelMode", GetWorldTransform() ? (IsLockRotation() + 1) : 0);
	}

	bool Text::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene)
	{
		// Gets if this should be rendered.
		if (m_model == nullptr || !IsEnabled() || GetAlpha() == 0.0f)
		{
			return false;
		}

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("UboObject", m_uniformObject);
		m_descriptorSet.Push("samplerColour", m_fontType->GetTexture());
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return false;
		}

		VkRect2D scissorRect = {};
		scissorRect.offset.x = static_cast<int32_t>(pipeline.GetWidth() * GetScissor().m_x) + 1;
		scissorRect.offset.y = static_cast<int32_t>(pipeline.GetHeight() * GetScissor().m_y) + 1;
		scissorRect.extent.width = static_cast<uint32_t>(pipeline.GetWidth() * GetScissor().m_z);
		scissorRect.extent.height = static_cast<uint32_t>(pipeline.GetHeight() * GetScissor().m_w);
		vkCmdSetScissor(commandBuffer.GetCommandBuffer(), 0, 1, &scissorRect);

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
		m_model->CmdRender(commandBuffer);
		return true;
	}

	void Text::SetString(const std::string &newString)
	{
		if (m_string != newString)
		{
			m_newString = newString;
		}
	}

	void Text::SetBorderDriver(IDriver<float> *borderDriver)
	{
		m_borderDriver.reset(borderDriver);
		m_solidBorder = true;
		m_glowBorder = false;
	}

	void Text::SetGlowDriver(IDriver<float> *glowDriver)
	{
		m_glowDriver.reset(glowDriver);
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
		if (m_string.empty())
		{
			m_model = nullptr;
			return;
		}

		// Creates mesh data.
		auto lines = CreateStructure();
		auto vertices = CreateQuad(lines);

		// Calculates the bounds and normalizes the vertices.
		Vector2 bounding = Vector2();
		NormalizeQuad(bounding, vertices);

		// Loads the mesh data.
		m_model = std::make_unique<Model>(vertices);
		GetRectangle().SetDimensions(Vector2(bounding.m_x, bounding.m_y));
	}

	std::vector<Text::Line> Text::CreateStructure()
	{
		std::vector<Line> lines = {};
		auto currentLine = Line(m_fontType->GetMetadata()->GetSpaceWidth(), m_maxWidth);
		auto currentWord = Word();

		auto formattedText = String::ReplaceAll(m_string, "\t", "	");
		auto textLines = String::Split(formattedText, "\n", true);

		for (uint32_t i = 0; i < textLines.size(); i++)
		{
			if (textLines.at(i).empty())
			{
				continue;
			}

			for (const auto &c : textLines.at(i))
			{
				auto ascii = static_cast<int32_t>(c);

				if (ascii == FontMetafile::SpaceAscii)
				{
					bool added = currentLine.AddWord(currentWord);

					if (!added)
					{
						lines.emplace_back(currentLine);
						currentLine = Line(m_fontType->GetMetadata()->GetSpaceWidth(), m_maxWidth);
						currentLine.AddWord(currentWord);
					}

					currentWord = Word();
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
				bool wordAdded = currentLine.AddWord(currentWord);
				lines.emplace_back(currentLine);
				currentLine = Line(m_fontType->GetMetadata()->GetSpaceWidth(), m_maxWidth);

				if (!wordAdded)
				{
					currentLine.AddWord(currentWord);
				}

				currentWord = Word();
			}
		}

		CompleteStructure(lines, currentLine, currentWord);
		return lines;
	}

	void Text::CompleteStructure(std::vector<Line> &lines, Line &currentLine, const Word &currentWord)
	{
		bool added = currentLine.AddWord(currentWord);

		if (!added)
		{
			lines.emplace_back(currentLine);
			currentLine = Line(m_fontType->GetMetadata()->GetSpaceWidth(), m_maxWidth);
			currentLine.AddWord(currentWord);
		}

		lines.emplace_back(currentLine);
	}

	std::vector<VertexModel> Text::CreateQuad(const std::vector<Line> &lines)
	{
		std::vector<VertexModel> vertices = {};
		m_numberLines = static_cast<uint32_t>(lines.size());

		float cursorX = 0.0f;
		float cursorY = 0.0f;
		auto lineOrder = static_cast<int32_t>(lines.size());

		for (const auto &line : lines)
		{
			switch (m_justify)
			{
			case Text::Justify::Left:
				cursorX = 0.0f;
				break;
			case Text::Justify::Centre:
				cursorX = (line.m_maxLength - line.m_currentLineLength) / 2.0f;
				break;
			case Text::Justify::Right:
				cursorX = line.m_maxLength - line.m_currentLineLength;
				break;
			case Text::Justify::Fully:
				cursorX = 0.0f;
				break;
			}

			for (const auto &word : line.m_words)
			{
				for (const auto &letter : word.m_characters)
				{
					AddVerticesForCharacter(cursorX, cursorY, letter, vertices);
					cursorX += m_kerning + letter.m_advanceX;
				}

				if (m_justify == Text::Justify::Fully && lineOrder > 1)
				{
					cursorX += (line.m_maxLength - line.m_currentWordsLength) / line.m_words.size();
				}
				else
				{
					cursorX += m_fontType->GetMetadata()->GetSpaceWidth();
				}
			}

			cursorY += m_leading + FontMetafile::LineHeight;
			lineOrder--;
		}

		return vertices;
	}

	void Text::AddVerticesForCharacter(const float &cursorX, const float &cursorY, const FontMetafile::Character &character, std::vector<VertexModel> &vertices)
	{
		float vertexX = cursorX + character.m_offsetX;
		float vertexY = cursorY + character.m_offsetY;
		float vertexMaxX = vertexX + character.m_sizeX;
		float vertexMaxY = vertexY + character.m_sizeY;

		float textureX = character.m_textureCoordX;
		float textureY = character.m_textureCoordY;
		float textureMaxX = character.m_maxTextureCoordX;
		float textureMaxY = character.m_maxTextureCoordY;

		AddVertex(vertexX, vertexY, textureX, textureY, vertices);
		AddVertex(vertexX, vertexMaxY, textureX, textureMaxY, vertices);
		AddVertex(vertexMaxX, vertexMaxY, textureMaxX, textureMaxY, vertices);
		AddVertex(vertexMaxX, vertexMaxY, textureMaxX, textureMaxY, vertices);
		AddVertex(vertexMaxX, vertexY, textureMaxX, textureY, vertices);
		AddVertex(vertexX, vertexY, textureX, textureY, vertices);
	}

	void Text::AddVertex(const float &vx, const float &vy, const float &tx, const float &ty, std::vector<VertexModel> &vertices)
	{
		vertices.emplace_back(VertexModel(Vector3(static_cast<float>(vx), static_cast<float>(vy), 0.0f), Vector2(static_cast<float>(tx), static_cast<float>(ty))));
	}

	void Text::NormalizeQuad(Vector2 &bounding, std::vector<VertexModel> &vertices)
	{
		float minX = +std::numeric_limits<float>::infinity();
		float minY = +std::numeric_limits<float>::infinity();
		float maxX = -std::numeric_limits<float>::infinity();
		float maxY = -std::numeric_limits<float>::infinity();

		for (const auto &vertex : vertices)
		{
			Vector3 position = vertex.GetPosition();

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

		if (m_justify == Text::Justify::Centre)
		{
			minX = 0.0f;
			maxX = m_maxWidth;
		}

	//	maxY = static_cast<float>(GetFontType()->GetMetadata()->GetMaxSizeY()) * m_numberLines;
		bounding = Vector2((maxX - minX) / 2.0f, (maxY - minX) / 2.0f);

		for (auto &vertex : vertices)
		{
			Vector3 position = Vector3((vertex.GetPosition().m_x - minX) / (maxX - minX), (vertex.GetPosition().m_y - minY) / (maxY - minY), 0.0f);
			vertex.SetPosition(position);
		}
	}
}

﻿#include "Text.hpp"

#include "Maths/Visual/DriverConstant.hpp"

namespace acid
{
Text::Text(UiObject *parent, const UiTransform &rectangle, const float &fontSize, std::string text, std::shared_ptr<FontType> fontType, const Justify &justify,
	const Colour &textColour, const float &kerning, const float &leading) :
	UiObject(parent, rectangle),
	m_numberLines(0),
	m_string(std::move(text)),
	m_justify(justify),
	m_fontType(std::move(fontType)),
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
	//SetScaleDriver(new DriverConstant<Vector2f>(Vector2f(fontSize)));
	LoadText();
}

void Text::UpdateObject()
{
	if (m_newString.has_value())
	{
		m_string = *m_newString;
		LoadText();
		m_newString = std::nullopt;
	}

	m_glowSize = m_glowDriver->Update(Engine::Get()->GetDelta());
	m_borderSize = m_borderDriver->Update(Engine::Get()->GetDelta());

	// Updates uniforms.
	m_uniformObject.Push("modelView", GetModelView());
	m_uniformObject.Push("alpha", GetScreenAlpha());

	m_uniformObject.Push("colour", m_textColour);
	m_uniformObject.Push("borderColour", m_borderColour);
	m_uniformObject.Push("borderSizes", Vector2f(GetTotalBorderSize(), GetGlowSize()));
	m_uniformObject.Push("edgeData", Vector2f(CalculateEdgeStart(), CalculateAntialiasSize()));
}

bool Text::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline)
{
	// Gets if this should be rendered.
	if (m_model == nullptr || m_fontType == nullptr || !IsEnabled())
	{
		return false;
	}

	// Updates descriptors.
	m_descriptorSet.Push("UniformObject", m_uniformObject);
	m_descriptorSet.Push("samplerColour", m_fontType->GetImage());
	bool updateSuccess = m_descriptorSet.Update(pipeline);

	if (!updateSuccess)
	{
		return false;
	}

	/*VkRect2D scissorRect = {};
	scissorRect.offset.x = static_cast<int32_t>(pipeline.GetRenderArea().GetExtent().m_x * GetScissor().m_x);
	scissorRect.offset.y = static_cast<int32_t>(pipeline.GetRenderArea().GetExtent().m_y * GetScissor().m_y);
	scissorRect.extent.width = static_cast<uint32_t>(pipeline.GetRenderArea().GetExtent().m_x * GetScissor().m_z);
	scissorRect.extent.height = static_cast<uint32_t>(pipeline.GetRenderArea().GetExtent().m_y * GetScissor().m_w);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);*/

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return m_model->CmdRender(commandBuffer);
}

void Text::SetString(const std::string &string)
{
	if (m_string != string)
	{
		m_newString = string;
	}
}

void Text::SetBorderDriver(Driver<float> *borderDriver)
{
	m_borderDriver.reset(borderDriver);
	m_solidBorder = true;
	m_glowBorder = false;
}

void Text::SetGlowDriver(Driver<float> *glowDriver)
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

float Text::GetTotalBorderSize() const
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

float Text::GetGlowSize() const
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

float Text::CalculateEdgeStart() const
{
	auto scale = GetScreenTransform().GetSize(); // (GetScreenDimensions() / GetTransform().GetDimensions()).MinComponent();
	auto size = 0.5f * scale.m_x;
	return 1.0f / 300.0f * size + 137.0f / 300.0f;
}

float Text::CalculateAntialiasSize() const
{
	auto scale = GetScreenTransform().GetSize(); // (GetScreenDimensions() / GetTransform().GetDimensions()).MinComponent();
	auto size = 0.5f * scale.m_x;
	size = (size - 1.0f) / (1.0f + size / 4.0f) + 1.0f;
	return 0.1f / size;
}

bool Text::IsLoaded() const
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
	Vector2f bounding;
	NormalizeQuad(bounding, vertices);

	// Loads the mesh data.
	m_model = std::make_unique<Model>(vertices);
	//GetTransform().SetSize(bounding);
}

std::vector<Text::Line> Text::CreateStructure() const
{
	float maxLength = GetTransform().GetSize().m_x;

	std::vector<Line> lines;
	auto currentLine = Line(m_fontType->GetMetadata()->GetSpaceWidth(), maxLength);
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
					currentLine = Line(m_fontType->GetMetadata()->GetSpaceWidth(), maxLength);
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
			currentLine = Line(m_fontType->GetMetadata()->GetSpaceWidth(), maxLength);

			if (!wordAdded)
			{
				currentLine.AddWord(currentWord);
			}

			currentWord = Word();
		}
	}

	CompleteStructure(lines, currentLine, currentWord, maxLength);
	return lines;
}

void Text::CompleteStructure(std::vector<Line> &lines, Line &currentLine, const Word &currentWord, const float &maxLength) const
{
	auto added = currentLine.AddWord(currentWord);

	if (!added)
	{
		lines.emplace_back(currentLine);
		currentLine = Line(m_fontType->GetMetadata()->GetSpaceWidth(), maxLength);
		currentLine.AddWord(currentWord);
	}

	lines.emplace_back(currentLine);
}

std::vector<VertexDefault> Text::CreateQuad(const std::vector<Line> &lines)
{
	std::vector<VertexDefault> vertices;
	m_numberLines = static_cast<uint32_t>(lines.size());

	auto cursorX = 0.0f;
	auto cursorY = 0.0f;
	auto lineOrder = static_cast<int32_t>(lines.size());

	for (const auto &line : lines)
	{
		switch (m_justify)
		{
		case Justify::Left:
			cursorX = 0.0f;
			break;
		case Justify::Centre:
			cursorX = (line.m_maxLength - line.m_currentLineLength) / 2.0f;
			break;
		case Justify::Right:
			cursorX = line.m_maxLength - line.m_currentLineLength;
			break;
		case Justify::Fully:
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

			if (m_justify == Justify::Fully && lineOrder > 1)
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

void Text::AddVerticesForCharacter(const float &cursorX, const float &cursorY, const FontMetafile::Character &character, std::vector<VertexDefault> &vertices)
{
	auto vertexX = cursorX + character.m_offsetX;
	auto vertexY = cursorY + character.m_offsetY;
	auto vertexMaxX = vertexX + character.m_sizeX;
	auto vertexMaxY = vertexY + character.m_sizeY;

	auto textureX = character.m_textureCoordX;
	auto textureY = character.m_textureCoordY;
	auto textureMaxX = character.m_maxTextureCoordX;
	auto textureMaxY = character.m_maxTextureCoordY;

	AddVertex(vertexX, vertexY, textureX, textureY, vertices);
	AddVertex(vertexMaxX, vertexY, textureMaxX, textureY, vertices);
	AddVertex(vertexMaxX, vertexMaxY, textureMaxX, textureMaxY, vertices);
	AddVertex(vertexMaxX, vertexMaxY, textureMaxX, textureMaxY, vertices);
	AddVertex(vertexX, vertexMaxY, textureX, textureMaxY, vertices);
	AddVertex(vertexX, vertexY, textureX, textureY, vertices);
}

void Text::AddVertex(const float &vx, const float &vy, const float &tx, const float &ty, std::vector<VertexDefault> &vertices)
{
	vertices.emplace_back(VertexDefault(Vector3f(vx, vy, 0.0f), Vector2f(tx, ty), Vector3f::Zero));
}

void Text::NormalizeQuad(Vector2f &bounding, std::vector<VertexDefault> &vertices) const
{
	auto min = Vector2f::PositiveInfinity;
	auto max = Vector2f::NegativeInfinity;

	for (const auto &vertex : vertices)
	{
		auto position = Vector2f(vertex.m_position);
		min = min.Min(position);
		max = max.Max(position);
	}

	if (m_justify == Justify::Centre)
	{
		min.m_x = 0.0f;
		max.m_x = GetTransform().GetSize().m_x;
	}

	//max.m_y = static_cast<float>(GetFontType()->GetMetadata()->GetMaxSizeY()) * m_numberLines;
	bounding = (max - min) / 2.0f;

	for (auto &vertex : vertices)
	{
		vertex.m_position = Vector3f((Vector2f(vertex.m_position) - min) / (max - min));
	}
}
}

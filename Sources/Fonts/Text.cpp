#include "Text.hpp"

#include "Maths/Visual/DriverConstant.hpp"
#include "Models/Shapes/ModelRectangle.hpp"

namespace acid {
Text::Text(UiObject *parent, const UiTransform &rectangle, float fontSize, std::string text, std::shared_ptr<FontType> fontType, const Justify &justify,
	const Colour &textColour, float kerning, float leading) :
	UiObject(parent, rectangle),
	m_lastSize(rectangle.GetSize()),
	m_fontSize(fontSize),
	m_string(std::move(text)),
	m_justify(justify),
	m_fontType(std::move(fontType)),
	m_kerning(kerning),
	m_leading(leading),
	m_textColour(textColour),
	m_glowDriver(std::make_unique<DriverConstant<float>>(0.0f)),
	m_borderDriver(std::make_unique<DriverConstant<float>>(0.0f)) {
	LoadText();
}

void Text::UpdateObject() {
	if (m_newString.has_value()) {
		m_string = *m_newString;
		m_newString = std::nullopt;
		LoadText();
	}

	if (GetScreenTransform().GetSize() != m_lastSize) {
		m_lastSize = GetScreenTransform().GetSize();
		LoadText();
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

bool Text::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline) {
	// Gets if this should be rendered.
	if (!m_model || !m_fontType || !IsEnabled()) {
		return false;
	}

	// Updates descriptors.
	m_descriptorSet.Push("UniformObject", m_uniformObject);
	m_descriptorSet.Push("samplerColour", m_fontType->GetImage());

	if (!m_descriptorSet.Update(pipeline)) {
		return false;
	}

	auto scissor = GetScissor();
	VkRect2D scissorRect = {};
	scissorRect.offset.x = scissor ? static_cast<int32_t>(scissor->m_x) : 0;
	scissorRect.offset.y = scissor ? static_cast<int32_t>(scissor->m_y) : 0;
	scissorRect.extent.width = scissor ? static_cast<int32_t>(scissor->m_z) : Window::Get()->GetSize().m_x;
	scissorRect.extent.height = scissor ? static_cast<int32_t>(scissor->m_w) : Window::Get()->GetSize().m_y;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return m_model->CmdRender(commandBuffer);
}

void Text::SetFontSize(float fontSize) {
	m_fontSize = fontSize;
	m_lastSize = {};
}

void Text::SetString(const std::string &string) {
	if (m_string != string) {
		m_newString = string;
	}
}

void Text::SetBorderDriver(std::unique_ptr<Driver<float>> &&borderDriver) {
	m_borderDriver = std::move(borderDriver);
	m_solidBorder = true;
	m_glowBorder = false;
}

void Text::SetGlowDriver(std::unique_ptr<Driver<float>> &&glowDriver) {
	m_glowDriver = std::move(glowDriver);
	m_solidBorder = false;
	m_glowBorder = true;
}

void Text::RemoveBorder() {
	m_solidBorder = false;
	m_glowBorder = false;
}

float Text::GetTotalBorderSize() const {
	if (m_solidBorder) {
		if (m_borderSize == 0.0f) {
			return 0.0f;
		}

		return CalculateEdgeStart() + m_borderSize;
	}

	if (m_glowBorder) {
		return CalculateEdgeStart();
	}

	return 0.0f;
}

float Text::GetGlowSize() const {
	if (m_solidBorder) {
		return CalculateAntialiasSize();
	}

	if (m_glowBorder) {
		return m_glowSize;
	}

	return 0.0f;
}

float Text::CalculateEdgeStart() const {
	auto scale = GetScale() * m_fontSize;
	auto size = 0.5f * scale.m_x;
	return 1.0f / 300.0f * size + 137.0f / 300.0f;
}

float Text::CalculateAntialiasSize() const {
	auto scale = GetScale() * m_fontSize;
	auto size = 0.5f * scale.m_x;
	size = (size - 1.0f) / (1.0f + size / 4.0f) + 1.0f;
	return 0.1f / size;
}

bool Text::IsLoaded() const {
	return !m_string.empty() && m_model;
}

void Text::LoadText() {
	if (m_string.empty()) {
		m_model = nullptr;
		return;
	}

	// Creates mesh data.
	auto lines = CreateStructure();
	auto vertices = CreateQuad(lines);

	// Loads the mesh data.
	m_model = std::make_unique<Model>(vertices);
}

std::vector<Text::Line> Text::CreateStructure() const {
	auto maxLength = m_lastSize.m_x;

	std::vector<Line> lines;
	Line currentLine(m_fontType->GetNode()->GetSpaceWidth(), maxLength);
	Word currentWord;

	auto formattedText = String::ReplaceAll(m_string, "\t", "	");
	auto textLines = String::Split(formattedText, '\n');

	for (uint32_t i = 0; i < textLines.size(); i++) {
		if (textLines.at(i).empty()) {
			continue;
		}

		for (const auto &c : textLines.at(i)) {
			auto ascii = static_cast<int32_t>(c);

			if (ascii == FontMetafile::SpaceAscii) {
				if (!currentLine.AddWord(currentWord)) {
					lines.emplace_back(currentLine);
					currentLine = {m_fontType->GetNode()->GetSpaceWidth(), maxLength};
					currentLine.AddWord(currentWord);
				}

				currentWord = {};
				continue;
			}

			if (auto character = m_fontType->GetNode()->GetCharacter(ascii); character) {
				currentWord.AddCharacter(*character, m_kerning);
			}
		}

		if (i != textLines.size() - 1) {
			auto wordAdded = currentLine.AddWord(currentWord);
			lines.emplace_back(currentLine);
			currentLine = {m_fontType->GetNode()->GetSpaceWidth(), maxLength};

			if (!wordAdded) {
				currentLine.AddWord(currentWord);
			}

			currentWord = {};
		}
	}

	CompleteStructure(lines, currentLine, currentWord, maxLength);
	return lines;
}

void Text::CompleteStructure(std::vector<Line> &lines, Line &currentLine, const Word &currentWord, float maxLength) const {
	auto added = currentLine.AddWord(currentWord);

	if (!added) {
		lines.emplace_back(currentLine);
		currentLine = {m_fontType->GetNode()->GetSpaceWidth(), maxLength};
		currentLine.AddWord(currentWord);
	}

	lines.emplace_back(currentLine);
}

std::vector<VertexDefault> Text::CreateQuad(const std::vector<Line> &lines) {
	std::vector<VertexDefault> vertices;
	m_numberLines = static_cast<uint32_t>(lines.size());

	float cursorX = 0.0f;
	float cursorY = 0.0f;
	auto lineOrder = static_cast<int32_t>(lines.size());

	for (const auto &line : lines) {
		switch (m_justify) {
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

		for (const auto &word : line.m_words) {
			for (const auto &letter : word.m_characters) {
				AddVerticesForCharacter(cursorX, cursorY, letter, vertices);
				cursorX += m_kerning + letter.m_advanceX;
			}

			if (m_justify == Justify::Fully && lineOrder > 1) {
				cursorX += (line.m_maxLength - line.m_currentWordsLength) / line.m_words.size();
			} else {
				cursorX += m_fontType->GetNode()->GetSpaceWidth();
			}
		}

		cursorY += m_leading + FontMetafile::LineHeight;
		lineOrder--;
	}

	return vertices;
}

void Text::AddVerticesForCharacter(float cursorX, float cursorY, const FontMetafile::Character &character, std::vector<VertexDefault> &vertices) {
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

void Text::AddVertex(float vx, float vy, float tx, float ty, std::vector<VertexDefault> &vertices) {
	vertices.emplace_back(VertexDefault({vx, vy, 0.0f}, {tx, ty}, {}));
}
}

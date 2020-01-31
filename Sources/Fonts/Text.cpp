#include "Text.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Models/Shapes/ModelRectangle.hpp"

namespace acid {
Text::Text(UiObject *parent, const UiTransform &rectangle, float fontSize, std::string text, std::shared_ptr<FontType> fontType, Justify justify,
	const Colour &textColour, float kerning, float leading) :
	UiObject(parent, rectangle),
	m_lastSize(rectangle.GetSize()),
	m_fontSize(fontSize),
	m_string(std::move(text)),
	m_justify(justify),
	m_fontType(std::move(fontType)),
	m_kerning(kerning),
	m_leading(leading),
	m_textColour(textColour) {
	LoadText();
}

void Text::UpdateObject() {
	m_dirty |= GetScreenTransform().GetSize() != m_lastSize;
	if (m_dirty)
		LoadText();

	// Updates uniforms.
	m_uniformObject.Push("modelView", GetModelView());
	m_uniformObject.Push("alpha", GetScreenAlpha());

	m_uniformObject.Push("colour", m_textColour);
	m_uniformObject.Push("scale", 0.5f * GetScreenTransform().GetSize().m_x / (float)m_fontType->GetSize());
}

bool Text::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline) {
	// Gets if this should be rendered.
	if (!m_model || !m_fontType || !IsEnabled()) {
		return false;
	}

	// Updates descriptors.
	m_descriptorSet.Push("UniformObject", m_uniformObject);
	m_descriptorSet.Push("samplerMsdf", m_fontType->GetImage());

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
	m_dirty |= m_fontSize != fontSize;
	m_fontSize = fontSize;
}

void Text::SetString(const std::string &string) {
	m_dirty |= m_string != string;
	m_string = string;
}

void Text::SetJustify(Justify justify) {
	m_dirty |= m_justify != justify;
	m_justify = justify;
}

void Text::SetKerning(float kerning) {
	m_dirty |= m_kerning != kerning;
	m_kerning = kerning;
}

void Text::SetLeading(float leading) {
	m_dirty |= m_leading != leading;
	m_leading = leading;
}

bool Text::IsLoaded() const {
	return !m_string.empty() && m_model;
}

void Text::LoadText() {
/*#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif*/

	if (m_string.empty()) {
		m_model = nullptr;
		return;
	}

	m_lastSize = GetScreenTransform().GetSize();
	
	// Creates mesh data.
	auto lines = CreateStructure();
	m_numberLines = static_cast<uint32_t>(lines.size());
	auto vertices = CreateQuad(lines);

	// Loads the mesh data.
	m_model = std::make_unique<Model>(vertices);
	m_dirty = false;

/*#if defined(ACID_DEBUG)
	Log::Out("Text mesh with ", m_string.length(), " chars created in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif*/
}

std::vector<Text::Line> Text::CreateStructure() const {
	/*auto maxLength = m_lastSize.m_x;// / m_fontSize * m_fontType->GetMetafile()->GetMaxAdvance();

	std::vector<Line> lines;
	Line currentLine(m_fontType->GetSpaceWidth(), maxLength);
	Word currentWord;

	auto formattedText = String::ReplaceAll(m_string, "\t", "	");
	auto textLines = String::Split(formattedText, '\n');

	for (uint32_t i = 0; i < textLines.size(); i++) {
		if (textLines.at(i).empty()) {
			continue;
		}

		for (const auto &c : textLines.at(i)) {
			auto ascii = static_cast<int32_t>(c);

			if (ascii == FontType::SpaceAscii) {
				if (!currentLine.AddWord(currentWord)) {
					lines.emplace_back(currentLine);
					currentLine = {m_fontType->GetSpaceWidth(), maxLength};
					currentLine.AddWord(currentWord);
				}

				currentWord = {};
				continue;
			}

			if (auto character = m_fontType->GetCharacter(ascii)) {
				currentWord.AddCharacter(*character, m_kerning);
			}
		}

		if (i != textLines.size() - 1) {
			auto wordAdded = currentLine.AddWord(currentWord);
			lines.emplace_back(currentLine);
			currentLine = {m_fontType->GetSpaceWidth(), maxLength};

			if (!wordAdded) {
				currentLine.AddWord(currentWord);
			}

			currentWord = {};
		}
	}

	CompleteStructure(lines, currentLine, currentWord, maxLength);
	return lines;*/
	return {};
}

void Text::CompleteStructure(std::vector<Line> &lines, Line &currentLine, const Word &currentWord, float maxLength) const {
	/*auto added = currentLine.AddWord(currentWord);

	if (!added) {
		lines.emplace_back(currentLine);
		currentLine = {m_fontType->GetSpaceWidth(), maxLength};
		currentLine.AddWord(currentWord);
	}

	lines.emplace_back(currentLine);*/
}

std::vector<VertexText> Text::CreateQuad(const std::vector<Line> &lines) const {
	/*std::vector<VertexText> vertices;

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
			for (const auto &letter : word.m_glyphs) {
				AddVerticesForGlyph(cursorX, cursorY, letter, vertices);
				cursorX += m_kerning + letter.m_advance;
			}

			if (m_justify == Justify::Fully && lineOrder > 1) {
				cursorX += (line.m_maxLength - line.m_currentWordsLength) / line.m_words.size();
			} else {
				cursorX += m_fontType->GetSpaceWidth();
			}
		}

		cursorY += m_leading + FontType::LineHeight;
		lineOrder--;
	}

	return vertices;*/
	return {};
}

void Text::AddVerticesForGlyph(float cursorX, float cursorY, const FontType::Glyph &glyph, std::vector<VertexText> &vertices) {
	/*auto vertexX = cursorX + glyph.m_offsetX;
	auto vertexY = cursorY + glyph.m_offsetY;
	auto vertexMaxX = vertexX + glyph.m_sizeX;
	auto vertexMaxY = vertexY + glyph.m_sizeY;

	auto textureX = glyph.m_textureCoordX;
	auto textureY = glyph.m_textureCoordY;
	auto textureMaxX = glyph.m_maxTextureCoordX;
	auto textureMaxY = glyph.m_maxTextureCoordY;

	AddVertex(vertexX, vertexY, textureX, textureY, vertices);
	AddVertex(vertexMaxX, vertexY, textureMaxX, textureY, vertices);
	AddVertex(vertexMaxX, vertexMaxY, textureMaxX, textureMaxY, vertices);
	AddVertex(vertexMaxX, vertexMaxY, textureMaxX, textureMaxY, vertices);
	AddVertex(vertexX, vertexMaxY, textureX, textureMaxY, vertices);
	AddVertex(vertexX, vertexY, textureX, textureY, vertices);*/
}

void Text::AddVertex(float vx, float vy, float tx, float ty, std::vector<VertexText> &vertices) {
	vertices.emplace_back(VertexText({vx, vy}, {tx, ty, 0.0f}));
}
}

#include "Text.hpp"

#include "Models/Shapes/ModelRectangle.hpp"

namespace acid {
void Text::UpdateObject() {
	dirty |= GetScreenSize() != lastSize;
	if (dirty)
		LoadText();

	// Updates uniforms.
	uniformObject.Push("modelView", GetModelView());
	uniformObject.Push("alpha", GetScreenAlpha());

	uniformObject.Push("colour", textColour);
	uniformObject.Push("scale", 0.5f * GetScreenSize().x / (float)fontType->GetSize());
}

bool Text::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline) {
	// Gets if this should be rendered.
	/*if (!model || !fontType || !IsEnabled())
		return false;

	// Updates descriptors.
	descriptorSet.Push("UniformObject", uniformObject);
	descriptorSet.Push("samplerMsdf", fontType->GetImage());

	if (!descriptorSet.Update(pipeline))
		return false;

	auto scissor = GetScissor();
	VkRect2D scissorRect = {};
	scissorRect.offset.x = scissor ? static_cast<int32_t>(scissor->x) : 0;
	scissorRect.offset.y = scissor ? static_cast<int32_t>(scissor->y) : 0;
	scissorRect.extent.width = scissor ? static_cast<int32_t>(scissor->z) : Window::Get()->GetSize().x;
	scissorRect.extent.height = scissor ? static_cast<int32_t>(scissor->w) : Window::Get()->GetSize().y;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

	// Draws the object.
	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return model->CmdRender(commandBuffer);*/
	return false;
}

void Text::SetFontSize(float fontSize) {
	dirty |= this->fontSize != fontSize;
	this->fontSize = fontSize;
}

void Text::SetString(const std::string &string) {
	dirty |= this->string != string;
	this->string = string;
}

void Text::SetJustify(Justify justify) {
	dirty |= this->justify != justify;
	this->justify = justify;
}

void Text::SetKerning(float kerning) {
	dirty |= this->kerning != kerning;
	this->kerning = kerning;
}

void Text::SetLeading(float leading) {
	dirty |= this->leading != leading;
	this->leading = leading;
}

void Text::SetFontType(const std::shared_ptr<FontType> &fontType) {
	dirty |= this->fontType == fontType;
	this->fontType = fontType;
}

bool Text::IsLoaded() const {
	return !string.empty() && model;
}

void Text::LoadText() {
/*#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif*/

	if (string.empty()) {
		model = nullptr;
		return;
	}

	lastSize = GetScreenSize();
	
	// Creates mesh data.
	auto lines = CreateStructure();
	numberLines = static_cast<uint32_t>(lines.size());
	auto vertices = CreateQuad(lines);

	vertices.emplace_back(Vector2f(0.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f));
	vertices.emplace_back(Vector2f(0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f));
	vertices.emplace_back(Vector2f(1.0f, 0.0f), Vector3f(1.0f, 1.0f, 0.0f));
	vertices.emplace_back(Vector2f(0.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f));
	vertices.emplace_back(Vector2f(1.0f, 0.0f), Vector3f(1.0f, 1.0f, 0.0f));
	vertices.emplace_back(Vector2f(1.0f, 1.0f), Vector3f(1.0f, 0.0f, 0.0f));

	// Loads the mesh data.
	model = std::make_unique<Model>(vertices);
	dirty = false;

/*#if defined(ACID_DEBUG)
	Log::Out("Text mesh with ", string.length(), " chars created in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif*/
}

std::vector<Text::Line> Text::CreateStructure() const {
	/*auto maxLength = lastSize.x;// / fontSize * fontType->GetMetafile()->GetMaxAdvance();

	std::vector<Line> lines;
	Line currentLine(fontType->GetSpaceWidth(), maxLength);
	Word currentWord;

	auto formattedText = String::ReplaceAll(string, "\t", "	");
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
					currentLine = {fontType->GetSpaceWidth(), maxLength};
					currentLine.AddWord(currentWord);
				}

				currentWord = {};
				continue;
			}

			if (auto character = fontType->GetCharacter(ascii)) {
				currentWord.AddCharacter(*character, kerning);
			}
		}

		if (i != textLines.size() - 1) {
			auto wordAdded = currentLine.AddWord(currentWord);
			lines.emplace_back(currentLine);
			currentLine = {fontType->GetSpaceWidth(), maxLength};

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
		currentLine = {fontType->GetSpaceWidth(), maxLength};
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
		switch (justify) {
		case Justify::Left:
			cursorX = 0.0f;
			break;
		case Justify::Centre:
			cursorX = (line.maxLength - line.currentLineLength) / 2.0f;
			break;
		case Justify::Right:
			cursorX = line.maxLength - line.currentLineLength;
			break;
		case Justify::Fully:
			cursorX = 0.0f;
			break;
		}

		for (const auto &word : line.words) {
			for (const auto &letter : word.glyphs) {
				AddVerticesForGlyph(cursorX, cursorY, letter, vertices);
				cursorX += kerning + letter.advance;
			}

			if (justify == Justify::Fully && lineOrder > 1) {
				cursorX += (line.maxLength - line.currentWordsLength) / line.words.size();
			} else {
				cursorX += fontType->GetSpaceWidth();
			}
		}

		cursorY += leading + FontType::LineHeight;
		lineOrder--;
	}

	return vertices;*/
	return {};
}

void Text::AddVerticesForGlyph(float cursorX, float cursorY, const FontType::Glyph &glyph, std::vector<VertexText> &vertices) {
	/*auto vertexX = cursorX + glyph.offsetX;
	auto vertexY = cursorY + glyph.offsetY;
	auto vertexMaxX = vertexX + glyph.sizeX;
	auto vertexMaxY = vertexY + glyph.sizeY;

	auto textureX = glyph.textureCoordX;
	auto textureY = glyph.textureCoordY;
	auto textureMaxX = glyph.maxTextureCoordX;
	auto textureMaxY = glyph.maxTextureCoordY;

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

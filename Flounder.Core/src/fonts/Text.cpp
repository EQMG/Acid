#include "Text.hpp"

#include "../engine/Engine.hpp"
#include "../visual/DriverConstant.hpp"
#include "UbosFonts.hpp"
#include "../devices/Display.hpp"

namespace Flounder
{
	Text::Text(UiObject *parent, const Vector3 &position, const float &fontSize, const Vector2 &pivot, const std::string &text, FontType *fontType, const Justify &justify, const float &maxWidth, const float &kerning, const float &leading) :
		UiObject(parent, position, Vector3(1.0f, 1.0f, RelativeScreen), pivot),
		m_uniformObject(new UniformBuffer(sizeof(UbosFonts::UboObject))),
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
	}

	void Text::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		if (m_model == nullptr || !IsVisible() || GetAlpha() == 0.0f)
		{
			return;
		}

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		UbosFonts::UboObject uboObject = {};
		uboObject.transform = Vector4(*GetScreenTransform());
		uboObject.colour = Colour(*m_textColour);
		uboObject.borderColour = Colour(*m_borderColour);
		uboObject.borderSizes = Vector2(GetTotalBorderSize(), GetGlowSize());
		uboObject.edgeData = Vector2(CalculateEdgeStart(), CalculateAntialiasSize());
		uboObject.alpha = GetAlpha();
		m_uniformObject->Update(&uboObject);

		VkRect2D scissorRect = {};
		scissorRect.offset.x = static_cast<uint32_t>(Display::Get()->GetWidth() * GetScissor()->m_x);
		scissorRect.offset.y = static_cast<uint32_t>(Display::Get()->GetHeight() * -GetScissor()->m_y);
		scissorRect.extent.width = static_cast<uint32_t>(Display::Get()->GetWidth() * GetScissor()->m_z);
		scissorRect.extent.height = static_cast<uint32_t>(Display::Get()->GetHeight() * GetScissor()->m_w);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissorRect);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ m_uniformObject->GetWriteDescriptor(0, descriptorSet), m_fontType->GetTexture()->GetWriteDescriptor(1, descriptorSet) }; // TODO: Modulaize this!
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

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
		const std::vector<Line> lines = CreateStructure(object);
		const std::vector<Vertex> vertices = CreateQuad(object, lines);

		// Calculates the bounds and zormalizes the verticies.
		Vector2 bounding = Vector2();
		std::vector<Vertex> verticesNormalized = NormalizeQuad(object, &bounding, vertices);

		// Loads mesh data to Vulkan.
		Model *model = new Model(verticesNormalized);
		object->m_model = model;
		object->SetDimensions(Vector3(bounding.m_x, bounding.m_y, RelativeScreen));
	}

	std::vector<Line> Text::CreateStructure(Text *object)
	{
		std::vector<Line> lines = std::vector<Line>();
		Line currentLine = Line(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->m_maxWidth);
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
					currentLine = Line(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->m_maxWidth);
					currentLine.AddWord(currentWord);
				}

				currentWord = Word();
				continue;
			}

			// TODO: TAB_ASCII

			if (ascii == Metafile::NEWLINE_ASCII)
			{
				lines.push_back(currentLine);
				currentLine = Line(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->m_maxWidth);
				currentLine.AddWord(currentWord);
				continue;
			}

			Character *character = object->GetFontType()->GetMetadata()->GetCharacter(ascii);

			if (character != nullptr)
			{
				currentWord.AddCharacter(*character, object->m_kerning);
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
			currentLine = Line(object->GetFontType()->GetMetadata()->GetSpaceWidth(), object->m_maxWidth);
			currentLine.AddWord(currentWord);
		}

		lines.push_back(currentLine);
	}

	std::vector<Vertex> Text::CreateQuad(Text *object, std::vector<Line> lines)
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
	//	object->m_numberLines = static_cast<int>(lines.size());
		double cursorX = 0.0;
		double cursorY = 0.0;
		int lineOrder = static_cast<int>(lines.size());

		for (auto line : lines)
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

			for (auto word : line.GetWords())
			{
				for (auto letter : word.GetCharacters())
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
			cursorY += object->m_leading + Metafile::LINE_HEIGHT;
			lineOrder--;
		}

		return vertices;
	}

	void Text::AddVerticesForCharacter(const double &cursorX, const double &cursorY, const Character &character, std::vector<Vertex> &vertices)
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

	void Text::AddVertex(const double &vx, const double &vy, const double &tx, const double &ty, std::vector<Vertex> &vertices)
	{
		const Vertex vertex = Vertex(Vector3(static_cast<float>(vx), static_cast<float>(vy), 0.0f), Vector2(static_cast<float>(tx), static_cast<float>(ty)));
		vertices.push_back(vertex);
	}

	std::vector<Vertex> Text::NormalizeQuad(Text *object, Vector2 *bounding, const std::vector<Vertex> &vertices)
	{
		std::vector<Vertex> vertices2 = std::vector<Vertex>();
		float minX = +INFINITY;
		float minY = +INFINITY;
		float maxX = -INFINITY;
		float maxY = -INFINITY;

		for (const auto vertex : vertices)
		{
			const Vector3 position = vertex.m_position;

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
		bounding->Set((maxX - minX) / 2.0f, (maxY - minX) / 2.0f);
		maxX -= minX;
		maxY -= minY;

		for (auto vertex : vertices)
		{
			Vector3 position = Vector3((vertex.m_position.m_x - minX) / maxX, (vertex.m_position.m_y - minY) / maxY, 0.0f);
			Vertex vertex2 = Vertex(position, Vector2(vertex.m_textures), Vector3(vertex.m_normal), Vector3(vertex.m_tangent));
			vertices2.push_back(vertex2);
		}

		return vertices2;
	}
}

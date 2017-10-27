#include "Text.hpp"

#include "../engine/Engine.hpp"
#include "../visual/DriverConstant.hpp"
#include "UbosFonts.hpp"
#include "../devices/Display.hpp"

namespace Flounder
{
	Text::Text(UiObject *parent, const Vector2 &position, const Vector2 &pivot, const std::string &text, const float &fontSize, FontType *fonttype, const float &maxLineLength, const Justify &justify) :
		UiObject(parent, position, Vector2(1.0f, 1.0f), pivot, true),
		m_uniformObject(new UniformBuffer(sizeof(UbosFonts::UboObject))),
		m_textString(text),
		m_textJustify(justify),
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
		if (IsLoaded() && m_newText != "")
		{
			delete m_model;

			m_textString = m_newText;
			LoadText(this);
			m_newText = "";
		}

		switch (m_textJustify)
		{
		case JustifyLeft:
		//	GetPositionOffsets()->Set(GetDimensions()->m_x * GetScreenDimensions()->m_x, 0.0f);
			break;
		case JustifyCentre:
		//	GetPositionOffsets()->Set(0.0f, 0.0f);
			break;
		case JustifyRight:
		//	GetPositionOffsets()->Set(-GetDimensions()->m_x * GetScreenDimensions()->m_x, 0.0f);
			break;
		case JustifyFully:
		//	GetPositionOffsets()->Set(0.0f, 0.0f);
			break;
		}

		m_glowSize = m_glowDriver->Update(Engine::Get()->GetDelta());
		m_borderSize = m_borderDriver->Update(Engine::Get()->GetDelta());
	}

	void Text::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline)
	{
		if (!IsVisible() || GetAlpha() == 0.0f)
		{
			return;
		}

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		UbosFonts::UboObject uboObject = {};
		uboObject.scissor = Vector4(*GetScissor());
		uboObject.transform = Vector4(
			GetScreenPosition()->m_x, GetScreenPosition()->m_y,
			GetScreenDimensions()->m_x, GetScreenDimensions()->m_y
		);
		uboObject.colour = Colour(*m_textColour);
		uboObject.alpha = GetAlpha();
		uboObject.borderColour = Vector4(*m_borderColour);
		uboObject.borderSizes = Vector2(GetTotalBorderSize(), GetGlowSize());
		uboObject.edgeData = Vector2(CalculateEdgeStart(), CalculateAntialiasSize());
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ m_uniformObject->GetWriteDescriptor(0, descriptorSet), m_fonttype->GetTexture()->GetWriteDescriptor(1, descriptorSet) }; // TODO: Modulaize this!
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(commandBuffer);
	}

	void Text::SetText(const std::string &newText)
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

	void Text::LoadText(Text *object)
	{
		// Creates mesh data.
		std::vector<Line> lines = CreateStructure(object);
		std::vector<Vertex> vertices = std::vector<Vertex>();
		CreateQuadVertices(object, lines, vertices);
		Vector2 dimensions = GetBounding(vertices);

		// Loads mesh data to Vulkan.
		Model *loaded = new Model(vertices);
		object->SetModel(loaded);
		object->SetDimensions(dimensions);
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

	void Text::CreateQuadVertices(Text *object, std::vector<Line> lines, std::vector<Vertex> &vertices)
	{
		object->SetNumberOfLines(static_cast<int>(lines.size()));
		double cursorX = 0.0;
		double cursorY = 0.0;

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
				cursorX = (line.GetMaxLength() - line.GetCurrentLineLength()) / 2.0;
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
					cursorX += letter.GetAdvanceX();
				}

				cursorX += object->GetFontType()->GetMetadata()->GetSpaceWidth();
			}

			cursorX = 0.0;
			cursorY += Metafile::LINE_HEIGHT;
		}
	}

	void Text::AddVerticesForCharacter(const double &cursorX, const double &cursorY, const Character &character, std::vector<Vertex> &vertices)
	{
		const double vertexX = cursorX + character.GetOffsetX();
		const double vertexY = cursorY + character.GetOffsetY();
		const double vertexMaxX = vertexX + character.GetSizeX();
		const double vertexMaxY = vertexY + character.GetSizeY();

		const double textureX = character.GetTextureCoordX();
		const double textureY = character.GetTextureCoordX();
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

	Vector2 Text::GetBounding(std::vector<Vertex> &vertices)
	{
		float minX = +INFINITY;
		float minY = +INFINITY;
		float maxX = -INFINITY;
		float maxY = -INFINITY;
		int i = 0;

		for (const auto v : vertices)
		{
			if (v.m_position.m_x < minX)
			{
				minX = v.m_position.m_x;
			}
			else if (v.m_position.m_x > minX)
			{
				maxX = v.m_position.m_x;
			}

			if (v.m_position.m_y < minY)
			{
				minY = v.m_position.m_y;
			}
			else if (v.m_position.m_y > minY)
			{
				maxY = v.m_position.m_y;
			}
		}

		return Vector2((minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
	}
}

#pragma once

#include "Resources/Resource.hpp"
#include "Graphics/Images/Image2d.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Outline.hpp"
#include "Graphics/Descriptors/DescriptorsHandler.hpp"
#include "Graphics/Buffers/InstanceBuffer.hpp"

namespace acid {
class Text;

/**
 * @brief Resource that is used when creating a font mesh.
 */
class ACID_EXPORT FontType : public Resource {
	friend class Text;
public:
	class Instance {
	public:
		static Shader::VertexInput GetVertexInput(uint32_t baseBinding = 0) {
			std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
				{baseBinding, sizeof(Instance), VK_VERTEX_INPUT_RATE_INSTANCE}
			};
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
				{0, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, m_rect)},
				{1, baseBinding, VK_FORMAT_R32_UINT, offsetof(Instance, m_glyphIndex)},
				{2, baseBinding, VK_FORMAT_R32_SFLOAT, offsetof(Instance, m_sharpness)},
				{3, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, m_colour)}
			};
			return {bindingDescriptions, attributeDescriptions};
		}

		Rect m_rect;
		uint32_t m_glyphIndex;
		float m_sharpness;
		Colour m_colour;
	};
	
	/**
	 * Creates a new font type, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The font type with the requested values.
	 */
	static std::shared_ptr<FontType> Create(const Node &node);

	/**
	 * Creates a new font type, or finds one with the same values.
	 * @param filename The font file to load glyphs for this type from.
	 * @return The font type with the requested values.
	 */
	static std::shared_ptr<FontType> Create(const std::filesystem::path &filename);

	/**
	 * Creates a new font type.
	 * @param filename The font file to load glyphs for this type from.
	 * @param load If this resource will be loaded immediately, otherwise {@link FontType#Load} can be called later.
	 */
	FontType(std::filesystem::path filename, bool load = true);

	void Update(const std::vector<Text *> &texts);
	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline);

	friend const Node &operator>>(const Node &node, FontType &fontType);
	friend Node &operator<<(Node &node, const FontType &fontType);

private:
	struct CellInfo {
		uint32_t pointOffset;
		uint32_t cellOffset;
		Vector2ui cellCount;
	};

	struct HostGlyphInfo {
		Rect bbox;
		float horiAdvance;
		float vertAdvance;
	};

	struct DeviceGlyphInfo {
		Rect bbox;
		//Rect cbox;
		CellInfo cellInfo;
	};

	void Load();

	static uint32_t AlignUint32(uint32_t value, uint32_t alignment);

	std::filesystem::path m_filename;

	std::map<wchar_t, uint32_t> m_charmap;
	std::vector<HostGlyphInfo> m_glyphInfos;

	uint32_t m_glyphDataSize = 0;
	uint32_t m_glyphInfoSize = 0;
	uint32_t m_glyphCellsSize = 0;
	uint32_t m_glyphPointsSize = 0;
	uint32_t m_glyphInfoOffset = 0;
	uint32_t m_glyphCellsOffset = 0;
	uint32_t m_glyphPointsOffset = 0;

	DescriptorsHandler m_descriptorSet;
	std::unique_ptr<StorageBuffer> m_storageGlyphs;
	std::unique_ptr<InstanceBuffer> m_instanceBuffer;

	uint32_t m_instances = 0;
};
}

#pragma once

#include "Resources/Resource.hpp"
#include "Maths/Colour.hpp"
#include "Renderer/Buffers/StorageBuffer.hpp"
#include "Renderer/Buffers/InstanceBuffer.hpp"
#include "Renderer/Descriptors/DescriptorsHandler.hpp"
#include "Renderer/Images/Image2d.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "FontMetafile.hpp"
#include "Outline.hpp"

namespace acid
{
class Text;

/**
 * @brief Resource that is used when creating a font mesh.
 */
class ACID_EXPORT FontType :
	public Resource
{
public:
	class Instance
	{
	public:
		static Shader::VertexInput GetVertexInput(const uint32_t &baseBinding = 0)
		{
			std::vector<VkVertexInputBindingDescription> bindingDescriptions = { 
				VkVertexInputBindingDescription{ baseBinding, sizeof(Instance), VK_VERTEX_INPUT_RATE_INSTANCE }
			};
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
				VkVertexInputAttributeDescription{ 0, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, m_rect) },
				VkVertexInputAttributeDescription{ 1, baseBinding, VK_FORMAT_R32_UINT, offsetof(Instance, m_glyphIndex) },
				VkVertexInputAttributeDescription{ 2, baseBinding, VK_FORMAT_R32_UINT, offsetof(Instance, m_sharpness) },
				VkVertexInputAttributeDescription{ 3, baseBinding, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Instance, m_colour) }
			};
			return Shader::VertexInput(bindingDescriptions, attributeDescriptions);
		}

		Rect m_rect;
		uint32_t m_glyphIndex;
		float m_sharpness;
		Colour m_colour;
	};

	/**
	 * Creates a new font type, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The font type with the requested values.
	 */
	static std::shared_ptr<FontType> Create(const Metadata &metadata);

	/**
	 * Creates a new font type, or finds one with the same values.
	 * @param filename The family file path that the texture atlases and character infos are contained in.
	 * @param style The style postfix to load as this type.
	 * @return The font type with the requested values.
	 */
	static std::shared_ptr<FontType> Create(const std::string &filename, const std::string &style = "Regular");

	/**
	 * Creates a new font type.
	 * @param filename The family file path that the texture atlases and character infos are contained in.
	 * @param style The style postfix to load as this type.
	 * @param load If this resource will be loaded immediately, otherwise {@link FontType#Load} can be called later.
	 */
	FontType(std::string filename, std::string style, const bool &load = true);

	void Update(const std::vector<Text *> &texts);

	bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline);

	void Load() override;

	const std::shared_ptr<Image2d> &GetImage() const { return m_image; }

	const FontMetafile *GetMetadata() const { return m_metadata.get(); }

	ACID_EXPORT friend const Metadata &operator>>(const Metadata &metadata, FontType &fontType);

	ACID_EXPORT friend Metadata &operator<<(Metadata &metadata, const FontType &fontType);

private:
	struct CellInfo
	{
		uint32_t pointOffset;
		uint32_t cellOffset;
		uint32_t cellCountX;
		uint32_t cellCountY;
	};

	struct HostGlyphInfo
	{
		Rect bbox;
		float horiAdvance;
		float vertAdvance;
	};

	struct DeviceGlyphInfo
	{
		Rect bbox;
		//Rect cbox;
		CellInfo cellInfo;
	};

	static uint32_t AlignUint32(const uint32_t &value, const uint32_t &alignment);

	void LoadFont(const std::string &filename);

	std::string m_filename;
	std::string m_style;

	std::map<wchar_t, uint32_t> m_charmap;
	std::vector<HostGlyphInfo> m_glyphInfos;

	uint32_t m_glyphDataSize{};
	uint32_t m_glyphInfoSize{};
	uint32_t m_glyphCellsSize{};
	uint32_t m_glyphPointsSize{};
	uint32_t m_glyphInfoOffset{};
	uint32_t m_glyphCellsOffset{};
	uint32_t m_glyphPointsOffset{};

	std::shared_ptr<Image2d> m_image;
	std::unique_ptr<FontMetafile> m_metadata;

	DescriptorsHandler m_descriptorSet;
	std::unique_ptr<StorageBuffer> m_storageGlyphs;
	std::unique_ptr<InstanceBuffer> m_instanceBuffer;

	uint32_t m_instances;
};
}

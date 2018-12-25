/*#pragma once

#include "Display/Display.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/Buffers/VertexBuffer.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Handlers/StorageHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Outline.hpp"

namespace acid
{
	struct CellInfo
	{
		uint32_t point_offset;
		uint32_t cell_offset;
		uint32_t cell_count_x;
		uint32_t cell_count_y;
	};

	struct GlyphInstance
	{
		Rect rect;
		uint32_t glyph_index;
		float sharpness;

		static VertexInput GetVertexInput(const uint32_t &binding = 0);
	};

	struct HostGlyphInfo
	{
		Rect bbox;
		float advance;
	};

	struct DeviceGlyphInfo
	{
		Rect bbox;
		CellInfo cell_info;
	};

	class RendererFonts2 :
		public IRenderer
	{
	private:
		Pipeline m_pipeline;
		DescriptorsHandler m_descriptorSet;
		StorageHandler m_storageBuffer;

		VkDeviceMemory m_instance_buffer_memory;
		VkBuffer m_instance_buffer;
		VkDeviceMemory m_instance_staging_buffer_memory;
		VkBuffer m_instance_staging_buffer;

		Outline *m_outlines;
		HostGlyphInfo *m_glyph_infos;

		void *m_glyph_data;
		uint32_t m_glyph_data_size;
		uint32_t m_glyph_info_size;
		uint32_t m_glyph_cells_size;
		uint32_t m_glyph_points_size;
		uint32_t m_glyph_info_offset;
		uint32_t m_glyph_cells_offset;
		uint32_t m_glyph_points_offset;

		GlyphInstance *m_glyph_instances;
		uint32_t m_glyph_instance_count;
	public:
		static const uint32_t MAX_VISIBLE_GLYPHS;
		static const uint32_t NUMBER_OF_GLYPHS;

		explicit RendererFonts2(const GraphicsStage &graphicsStage);

		void Update();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera) override;
	private:
		static uint32_t align_uint32(const uint32_t &value, const uint32_t &alignment);

		void LoadFont();

		void NewFace(FT_Library *library, const std::string &filename, const signed long &faceIndex, FT_Face *aface);

		void create_instance_buffer();
		VkDeviceMemory alloc_required_memory(VkMemoryRequirements *req, VkMemoryPropertyFlags flags);
		void create_buffer_with_memory(VkBufferCreateInfo *ci, VkMemoryPropertyFlags flags, VkDeviceMemory *memory, VkBuffer *buffer);

		void BeginText();

		void AppendText(float x, float y, float scale, const std::string &text);

		void EndText(const CommandBuffer &commandBuffer);
	};
}*/

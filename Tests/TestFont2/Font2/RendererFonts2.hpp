#pragma once

#include <Renderer/RenderPipeline.hpp>
#include <Renderer/Pipelines/PipelineGraphics.hpp>
#include <Renderer/Handlers/DescriptorsHandler.hpp>
#include <Renderer/Handlers/StorageHandler.hpp>
#include <Renderer/Handlers/UniformHandler.hpp>
#include "Outline.hpp"

namespace acid
{
	class RendererFonts2 :
		public RenderPipeline
	{
	public:
		explicit RendererFonts2(const Pipeline::Stage &pipelineStage);

		void Render(const CommandBuffer &commandBuffer) override;
	private:
		void load_font();
		static VkDeviceMemory alloc_required_memory(VkMemoryRequirements *req, VkMemoryPropertyFlags flags);
		static void create_buffer_with_memory(VkBufferCreateInfo *ci, VkMemoryPropertyFlags flags, VkDeviceMemory *memory, VkBuffer *buffer);
		void create_instance_buffer();
		void begin_text();
		void end_text(const CommandBuffer &commandBuffer);
		void append_text(float x, float y, float scale, std::string text);

#define MAX_VISIBLE_GLYPHS 4096
#define NUMBER_OF_GLYPHS 96

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

			static Shader::VertexInput GetVertexInput(const uint32_t &binding = 0);
		};

		struct HostGlyphInfo
		{
			Rect bbox;
			float advance;
		};

		struct DeviceGlyphInfo
		{
			Rect bbox;
			//Rect cbox;
			CellInfo cell_info;
		};

		PipelineGraphics m_pipeline;
		DescriptorsHandler m_descriptorSet;
		StorageHandler m_storageBuffer;


		Outline *outlines;
		HostGlyphInfo *glyph_infos;
		
		void *glyph_data;
		uint32_t glyph_data_size;
		uint32_t glyph_info_size;
		uint32_t glyph_cells_size;
		uint32_t glyph_points_size;
		uint32_t glyph_info_offset;
		uint32_t glyph_cells_offset;
		uint32_t glyph_points_offset;

		GlyphInstance *glyph_instances;
		uint32_t glyph_instance_count;

		VkDeviceMemory storage_buffer_memory;
		VkBuffer storage_buffer;
		VkDeviceMemory instance_buffer_memory;
		VkBuffer instance_buffer;
		VkDeviceMemory instance_staging_buffer_memory;
		VkBuffer instance_staging_buffer;
	};
}

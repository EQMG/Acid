#pragma once

#include "Resources/Resource.hpp"
#include "Textures/Texture.hpp"
#include "Maths/Colour.hpp"
#include "Renderer/Buffers/StorageBuffer.hpp"
#include "Renderer/Buffers/InstanceBuffer.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "FontMetafile.hpp"
#include "Outline.hpp"

namespace acid
{
	class Text;

	/// <summary>
	/// A loader capable of loading font data into a instance of a text mesh.
	/// </summary>
	class ACID_EXPORT FontType :
		public Resource
	{
	public:
		/// <summary>
		/// Will find an existing font type with the same values, or create a new font type.
		/// </summary>
		/// <param name="metadata"> The metadata to decode values from. </param>
		static std::shared_ptr<FontType> Create(const Metadata &metadata);

		/// <summary>
		/// Will find an existing font type with the same values, or create a new font type.
		/// </summary>
		/// <param name="filename"> The family file path that the texture atlases and character infos are contained in. </param>
		/// <param name="style"> The style selected to load as this type. </param>
		static std::shared_ptr<FontType> Create(const std::string &filename, const std::string &style = "Regular");

		/// <summary>
		/// Creates a new font type.
		/// </summary>
		/// <param name="filename"> The family file path that the texture atlases and character infos are contained in. </param>
		/// <param name="style"> The style selected to load as this type. </param>
		/// <param name="load"> If this resource will load immediately, otherwise <seealso cref="#Load()"/> can be called. </param>
		FontType(std::string filename, std::string style, const bool &load = true);

		void Update(const std::vector<Text *> &texts);

		bool CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline);

		void Load() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		static Shader::VertexInput GetVertexInput(const uint32_t &binding = 0);

		const std::shared_ptr<Texture> &GetTexture() const { return m_texture; }

		const FontMetafile *GetMetadata() const { return m_metadata.get(); }
	private:
		struct CellInfo
		{
			uint32_t pointOffset;
			uint32_t cellOffset;
			uint32_t cellCountX;
			uint32_t cellCountY;
		};

		struct GlyphInstance
		{
			Rect rect;
			uint32_t glyphIndex;
			float sharpness;
			Colour colour;
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
			//	Rect cbox;
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

		std::shared_ptr<Texture> m_texture;
		std::unique_ptr<FontMetafile> m_metadata;

		DescriptorsHandler m_descriptorSet;
		std::unique_ptr<StorageBuffer> m_storageGlyphs;
		std::unique_ptr<InstanceBuffer> m_instanceBuffer;

		GlyphInstance *m_glyphInstances;
		uint32_t m_instances;
	};
}

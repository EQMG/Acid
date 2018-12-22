#include "RendererFonts2.hpp"

#include "Models/VertexModel.hpp"

namespace acid
{
	RendererFonts2::RendererFonts2(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Fonts2/Font.vert", "Shaders/Fonts2/Font.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_READ_WRITE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {}))),
		m_descriptorSet(DescriptorsHandler()),
		m_storageInstances(StorageHandler()),
		m_storageCells(StorageHandler()),
		m_storagePoints(StorageHandler())
	{
	}

	void RendererFonts2::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera)
	{
	/*//	m_storageInstances.Push("glyphs", *glyphs.data(), sizeof(GlyphInfo) * GLYPH_INSTANCES);

		m_descriptorSet.Push("GlyphBuffer", m_storageInstances);
		m_descriptorSet.Push("CellBuffer", m_storageCells);
		m_descriptorSet.Push("PointBuffer", m_storagePoints);
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
		//	return false;
		}

		// Draws the instanced objects.
		m_descriptorSet.BindDescriptor(commandBuffer);*/
	}
}

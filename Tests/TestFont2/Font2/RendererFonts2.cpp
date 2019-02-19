#include "RendererFonts2.hpp"

#include <Models/VertexModel.hpp>
#include <Models/Shapes/ModelRectangle.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>
#include <Renderer/Renderer.hpp>

namespace acid
{
	RendererFonts2::RendererFonts2(const Pipeline::Stage &pipelineStage) :
		RenderPipeline(pipelineStage),
		m_pipeline(PipelineGraphics(pipelineStage, {"Shaders/Fonts2/Font.vert", "Shaders/Fonts2/Font.frag"}, {VertexModel::GetVertexInput()},
		                            PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, false, {})),
		m_descriptorSet(DescriptorsHandler())
	{
		Log::Out("%s\n", m_pipeline.GetShaderProgram()->ToString().c_str());

		//	std::string filename = "Resources/Engine/Fonts/Alice-Regular.ttf";
		//	std::string filename = "Resources/Engine/Fonts/marediv.ttf";
		std::string filename = "Resources/Engine/Fonts/Lobster-Regular.ttf";
		//	std::string filename = "Resources/Engine/Fonts/LobsterTwo-Bold.ttf";
		//	std::string filename = "Resources/Engine/Fonts/LobsterTwo-BoldItalic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/LobsterTwo-Italic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/LobsterTwo-Regular.ttf";
		//	std::string filename = "Resources/Engine/Fonts/OpenSans-Bold.ttf";
		//	std::string filename = "Resources/Engine/Fonts/OpenSans-BoldItalic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/OpenSans-ExtraBold.ttf";
		//	std::string filename = "Resources/Engine/Fonts/OpenSans-ExtraBoldItalic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/OpenSans-Italic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/OpenSans-Light.ttf";
		//	std::string filename = "Resources/Engine/Fonts/OpenSans-LightItalic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/OpenSans-Regular.ttf";
		//	std::string filename = "Resources/Engine/Fonts/OpenSans-SemiBold.ttf";
		//	std::string filename = "Resources/Engine/Fonts/OpenSans-SemiBoldItalic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-Black.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-BlackItalic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-Bold.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-BoldItalic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-Italic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-Light.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-LightItalic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-Medium.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-MediumItalic.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-Regular.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-Thin.ttf";
		//	std::string filename = "Resources/Engine/Fonts/Roboto-ThinItalic.ttf";
		//	LoadFont(filename);
	}

	void RendererFonts2::Render(const CommandBuffer &commandBuffer)
	{
		/*m_pipeline.BindPipeline(commandBuffer);

		// Updates descriptors.
	//	m_descriptorSet.Push("Instances", m_storageInstances);
	//	m_descriptorSet.Push("GlyphBuffer", m_storageBuffer, OffsetSize(glyph_info_offset, glyph_info_size));
	//	m_descriptorSet.Push("CellBuffer", m_storageBuffer, OffsetSize(glyph_cells_offset, glyph_cells_size));
	//	m_descriptorSet.Push("PointBuffer", m_storageBuffer, OffsetSize(glyph_points_offset, glyph_points_size));
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);

	//	m_model->CmdRender(commandBuffer, glyph_instance_count);*/
	}
}

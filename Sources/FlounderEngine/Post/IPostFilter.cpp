#include "IPostFilter.hpp"

#include "../Devices/Display.hpp"
#include "../Models/Shapes/ShapeRectangle.hpp"

namespace Flounder
{
	IPostFilter::IPostFilter(const std::string &fragmentShader, const GraphicsStage &graphicsStage, const std::vector<std::string> &defines) :
		m_descriptorSet(nullptr),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Filters/Default.vert", fragmentShader},
			VertexModel::GetBindingDescriptions(), PIPELINE_POLYGON_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), defines)),
		m_model(ShapeRectangle::Resource(-1.0f, 1.0f))
	{
	}

	IPostFilter::~IPostFilter()
	{
		delete m_model;
		delete m_pipeline;
	}

	void IPostFilter::Render(const VkCommandBuffer &commandBuffer)
	{
	}
}

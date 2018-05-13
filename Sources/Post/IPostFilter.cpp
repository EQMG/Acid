#include "Post/IPostFilter.hpp"

#include "Models/Shapes/ShapeRectangle.hpp"

namespace fl
{
	IPostFilter::IPostFilter(const std::string &fragmentShader, const GraphicsStage &graphicsStage, const std::vector<PipelineDefine> &defines) :
		m_descriptorSet(new DescriptorsHandler()),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Filters/Default.vert", fragmentShader},
			VertexModel::GetVertexInput(), PIPELINE_POLYGON_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), defines)),
		m_model(ShapeRectangle::Resource(-1.0f, 1.0f))
	{
	}

	IPostFilter::~IPostFilter()
	{
		delete m_descriptorSet;
		delete m_pipeline;
		delete m_model;
	}

	void IPostFilter::Render(const VkCommandBuffer &commandBuffer)
	{
	}
}

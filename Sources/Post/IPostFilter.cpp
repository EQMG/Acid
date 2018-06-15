#include "Post/IPostFilter.hpp"

#include "Models/Shapes/ShapeRectangle.hpp"

namespace fl
{
	IPostFilter::IPostFilter(const GraphicsStage &graphicsStage, const std::vector<std::string> &shaderStages, const std::vector<PipelineDefine> &defines) :
		IRenderer(graphicsStage),
		m_descriptorSet(DescriptorsHandler()),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate(shaderStages, VertexModel::GetVertexInput(),
			PIPELINE_MODE_POLYGON_NO_DEPTH, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_BACK), defines)),
		m_model(ShapeRectangle::Resource(-1.0f, 1.0f))
	{
	}

	IPostFilter::~IPostFilter()
	{
	}
}

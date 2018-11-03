#include "Post/IPostFilter.hpp"

#include "Models/Shapes/ModelRectangle.hpp"
#include "Models/VertexModel.hpp"
#include "IPostFilter.hpp"

namespace acid
{
	IPostFilter::IPostFilter(const GraphicsStage &graphicsStage, const std::vector<std::string> &shaderStages, const std::vector<PipelineDefine> &defines) :
		IRenderer(graphicsStage),
		m_descriptorSet(DescriptorsHandler()),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate(shaderStages, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_NONE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, defines))),
		m_model(ModelRectangle::Resource(-1.0f, 1.0f)),
		m_attachments(std::map<std::string, IDescriptor *>())
	{
	}

	IDescriptor *IPostFilter::GetAttachment(const std::string &name, IDescriptor *descriptor) const
	{
		auto it = m_attachments.find(name);

		if (it == m_attachments.end())
		{
			return descriptor;
		}

		return it->second;
	}

	IDescriptor *IPostFilter::GetAttachment(const std::string &name, const std::string &rendererAttachment) const
	{
		auto it = m_attachments.find(name);

		if (it == m_attachments.end())
		{
			return Renderer::Get()->GetAttachment(rendererAttachment);
		}

		return it->second;
	}

	void IPostFilter::SetAttachment(const std::string &name, IDescriptor *descriptor)
	{
		auto it = m_attachments.find(name);

		if (it == m_attachments.end())
		{
			m_attachments.emplace(name, descriptor);
			return;
		}

		it->second = descriptor;
	}

	bool IPostFilter::RemoveAttachment(const std::string &name)
	{
		auto it = m_attachments.find(name);

		if (it != m_attachments.end())
		{
			m_attachments.erase(it);
			return true;
		}

		return false;
	}
}

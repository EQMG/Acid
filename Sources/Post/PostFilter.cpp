#include "Post/PostFilter.hpp"

#include "Models/Shapes/ModelRectangle.hpp"
#include "Models/VertexModel.hpp"
#include "PostFilter.hpp"

namespace acid
{
	uint32_t PostFilter::GLOBAL_SWITCHING = 0;

	PostFilter::PostFilter(const GraphicsStage &graphicsStage, const std::vector<std::string> &shaderStages, const std::vector<ShaderDefine> &defines) :
		IRenderer(graphicsStage),
		m_descriptorSet(DescriptorsHandler()),
		m_pipeline(Pipeline(graphicsStage, shaderStages, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_NONE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, defines)),
		m_model(ModelRectangle::Resource(-1.0f, 1.0f)),
		m_attachments(std::map<std::string, IDescriptor *>())
	{
	}

	IDescriptor *PostFilter::GetAttachment(const std::string &descriptorName, IDescriptor *descriptor) const
	{
		auto it = m_attachments.find(descriptorName);

		if (it == m_attachments.end())
		{
			return descriptor;
		}

		return it->second;
	}

	IDescriptor *PostFilter::GetAttachment(const std::string &descriptorName, const std::string &rendererAttachment) const
	{
		auto it = m_attachments.find(descriptorName);

		if (it == m_attachments.end())
		{
			return Renderer::Get()->GetAttachment(rendererAttachment);
		}

		return it->second;
	}

	void PostFilter::SetAttachment(const std::string &descriptorName, IDescriptor *descriptor)
	{
		auto it = m_attachments.find(descriptorName);

		if (it == m_attachments.end())
		{
			m_attachments.emplace(descriptorName, descriptor);
			return;
		}

		it->second = descriptor;
	}

	bool PostFilter::RemoveAttachment(const std::string &name)
	{
		auto it = m_attachments.find(name);

		if (it != m_attachments.end())
		{
			m_attachments.erase(it);
			return true;
		}

		return false;
	}

	void PostFilter::PushConditional(const std::string &descriptorName1, const std::string &descriptorName2, const std::string &rendererAttachment1, const std::string &rendererAttachment2)
	{
		auto it1 = m_attachments.find(descriptorName1);
		auto it2 = m_attachments.find(descriptorName1);

		if (it1 != m_attachments.end() || it2 != m_attachments.end())
		{
			m_descriptorSet.Push(descriptorName1, GetAttachment(descriptorName1, rendererAttachment1));
			m_descriptorSet.Push(descriptorName2, GetAttachment(descriptorName2, rendererAttachment1));
			return;
		}
		else if (it1 == m_attachments.end() && it2 != m_attachments.end())
		{
			m_descriptorSet.Push(descriptorName1, Renderer::Get()->GetAttachment(GLOBAL_SWITCHING % 2 == 1 ? rendererAttachment1 : rendererAttachment2));
			m_descriptorSet.Push(descriptorName2, GetAttachment(descriptorName2, rendererAttachment1));
			return;
		}
		else if (it1 != m_attachments.end() && it2 == m_attachments.end())
		{
			m_descriptorSet.Push(descriptorName1, GetAttachment(descriptorName1, rendererAttachment1));
			m_descriptorSet.Push(descriptorName2, Renderer::Get()->GetAttachment(GLOBAL_SWITCHING % 2 == 1 ? rendererAttachment1 : rendererAttachment2));
			return;
		}

		if (GLOBAL_SWITCHING % 2 == 1)
		{
			m_descriptorSet.Push(descriptorName1, Renderer::Get()->GetAttachment(rendererAttachment1));
			m_descriptorSet.Push(descriptorName2, Renderer::Get()->GetAttachment(rendererAttachment2));
		}
		else
		{
			m_descriptorSet.Push(descriptorName1, Renderer::Get()->GetAttachment(rendererAttachment2));
			m_descriptorSet.Push(descriptorName2, Renderer::Get()->GetAttachment(rendererAttachment1));
		}

		GLOBAL_SWITCHING++;
	}
}

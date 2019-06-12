#include "PipelineMaterial.hpp"

#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"

namespace acid
{
std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Metadata &metadata)
{
	/*auto resource{Resources::Get()->Find(metadata)};

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<PipelineMaterial>(resource);
	}

	auto result{std::make_shared<PipelineMaterial>()};
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;*/
	return nullptr;
}

std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate)
{
	PipelineMaterial temp{pipelineStage, pipelineCreate};
	Metadata metadata;
	metadata << temp;

	auto resource{Resources::Get()->Find(metadata)};

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<PipelineMaterial>(resource);
	}

	auto result{std::make_shared<PipelineMaterial>(pipelineStage, pipelineCreate)};
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	//metadata >> *result;
	//result->Load();
	return result;
}

PipelineMaterial::PipelineMaterial(Pipeline::Stage pipelineStage, PipelineGraphicsCreate pipelineCreate) :
	m_pipelineStage{std::move(pipelineStage)},
	m_pipelineCreate{std::move(pipelineCreate)}
{
}

bool PipelineMaterial::BindPipeline(const CommandBuffer &commandBuffer)
{
	auto renderStage{Graphics::Get()->GetRenderStage(m_pipelineStage.first)};

	if (renderStage == nullptr)
	{
		return false;
	}

	if (m_renderStage != renderStage)
	{
		m_renderStage = renderStage;
		m_pipeline.reset(m_pipelineCreate.Create(m_pipelineStage));
	}

	m_pipeline->BindPipeline(commandBuffer);
	return true;
}

const Metadata &operator>>(const Metadata &metadata, PipelineMaterial &pipeline)
{
	metadata.GetChild("renderpass", pipeline.m_pipelineStage.first);
	metadata.GetChild("subpass", pipeline.m_pipelineStage.second);
	metadata.GetChild("pipelineCreate", pipeline.m_pipelineCreate);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const PipelineMaterial &pipeline)
{
	metadata.SetChild("renderpass", pipeline.m_pipelineStage.first);
	metadata.SetChild("subpass", pipeline.m_pipelineStage.second);
	metadata.SetChild("pipelineCreate", pipeline.m_pipelineCreate);
	return metadata;
}
}

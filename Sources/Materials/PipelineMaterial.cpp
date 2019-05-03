#include "PipelineMaterial.hpp"

#include "Resources/Resources.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Metadata &metadata)
{
	/*auto resource = Resources::Get()->Find(metadata);

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<PipelineMaterial>(resource);
	}

	auto result = std::make_shared<PipelineMaterial>();
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;*/
	return nullptr;
}

std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate)
{
	auto temp = PipelineMaterial(pipelineStage, pipelineCreate);
	Metadata metadata = Metadata();
	metadata << temp;

	auto resource = Resources::Get()->Find(metadata);

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<PipelineMaterial>(resource);
	}

	auto result = std::make_shared<PipelineMaterial>(pipelineStage, pipelineCreate);
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	//metadata >> *result;
	//result->Load();
	return result;
}

PipelineMaterial::PipelineMaterial(Pipeline::Stage pipelineStage, PipelineGraphicsCreate pipelineCreate) :
	m_pipelineStage(std::move(pipelineStage)),
	m_pipelineCreate(std::move(pipelineCreate)),
	m_renderStage(nullptr),
	m_pipeline(nullptr)
{
}

bool PipelineMaterial::BindPipeline(const CommandBuffer &commandBuffer)
{
	auto renderStage = Renderer::Get()->GetRenderStage(m_pipelineStage.first);

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
	metadata.GetChild("Renderpass", pipeline.m_pipelineStage.first);
	metadata.GetChild("Subpass", pipeline.m_pipelineStage.second);
	metadata.GetChild("Pipeline Create", pipeline.m_pipelineCreate);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const PipelineMaterial &pipeline)
{
	metadata.SetChild("Renderpass", pipeline.m_pipelineStage.first);
	metadata.SetChild("Subpass", pipeline.m_pipelineStage.second);
	metadata.SetChild("Pipeline Create", pipeline.m_pipelineCreate);
	return metadata;
}
}

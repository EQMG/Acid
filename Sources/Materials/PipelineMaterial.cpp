#include "PipelineMaterial.hpp"

#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"

namespace acid {
std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<PipelineMaterial>(node))
		return resource;

	auto result = std::make_shared<PipelineMaterial>();
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	//result->Load();
	return result;
}

std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate) {
	PipelineMaterial temp(pipelineStage, pipelineCreate);
	Node node;
	node << temp;
	return Create(node);
}

PipelineMaterial::PipelineMaterial(Pipeline::Stage pipelineStage, PipelineGraphicsCreate pipelineCreate) :
	m_pipelineStage(std::move(pipelineStage)),
	m_pipelineCreate(std::move(pipelineCreate)) {
}

bool PipelineMaterial::BindPipeline(const CommandBuffer &commandBuffer) {
	auto renderStage = Graphics::Get()->GetRenderStage(m_pipelineStage.first);

	if (!renderStage) {
		return false;
	}

	if (m_renderStage != renderStage) {
		m_renderStage = renderStage;
		m_pipeline.reset(m_pipelineCreate.Create(m_pipelineStage));
	}

	m_pipeline->BindPipeline(commandBuffer);
	return true;
}

const Node &operator>>(const Node &node, PipelineMaterial &pipeline) {
	node["renderpass"].Get(pipeline.m_pipelineStage.first);
	node["subpass"].Get(pipeline.m_pipelineStage.second);
	node["pipelineCreate"].Get(pipeline.m_pipelineCreate);
	return node;
}

Node &operator<<(Node &node, const PipelineMaterial &pipeline) {
	node["renderpass"].Set(pipeline.m_pipelineStage.first);
	node["subpass"].Set(pipeline.m_pipelineStage.second);
	node["pipelineCreate"].Set(pipeline.m_pipelineCreate);
	return node;
}
}

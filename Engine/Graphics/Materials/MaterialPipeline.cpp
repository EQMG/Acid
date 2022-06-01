#include "MaterialPipeline.hpp"

#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"

namespace acid {
std::shared_ptr<MaterialPipeline> MaterialPipeline::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<MaterialPipeline>(node))
		return resource;

	auto result = std::make_shared<MaterialPipeline>();
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	//result->Load();
	return result;
}

std::shared_ptr<MaterialPipeline> MaterialPipeline::Create(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate) {
	MaterialPipeline temp(pipelineStage, pipelineCreate);
	Node node;
	node << temp;
	return Create(node);
}

MaterialPipeline::MaterialPipeline(Pipeline::Stage pipelineStage, PipelineGraphicsCreate pipelineCreate) :
	pipelineStage(std::move(pipelineStage)),
	pipelineCreate(std::move(pipelineCreate)) {
}

bool MaterialPipeline::BindPipeline(const CommandBuffer &commandBuffer) {
	auto renderStage = Graphics::Get()->GetRenderStage(pipelineStage.first);

	if (!renderStage)
		return false;

	if (this->renderStage != renderStage) {
		this->renderStage = renderStage;
		pipeline.reset(pipelineCreate.Create(pipelineStage));
	}

	pipeline->BindPipeline(commandBuffer);
	return true;
}

const Node &operator>>(const Node &node, MaterialPipeline &pipeline) {
	node["renderpass"].Get(pipeline.pipelineStage.first);
	node["subpass"].Get(pipeline.pipelineStage.second);
	node["pipelineCreate"].Get(pipeline.pipelineCreate);
	return node;
}

Node &operator<<(Node &node, const MaterialPipeline &pipeline) {
	node["renderpass"].Set(pipeline.pipelineStage.first);
	node["subpass"].Set(pipeline.pipelineStage.second);
	node["pipelineCreate"].Set(pipeline.pipelineCreate);
	return node;
}
}

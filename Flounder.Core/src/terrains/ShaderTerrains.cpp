#include "ShaderTerrains.hpp"

#include "../models/Vertex.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"

namespace Flounder
{
	DescriptorType ShaderTerrains::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	DescriptorType ShaderTerrains::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL);
	Descriptor ShaderTerrains::descriptor = Descriptor::Create({ typeUboScene, typeUboObject });
	InputState ShaderTerrains::inputState = InputState::Create(Vertex::GetBindingDescriptions(), Vertex::GetAttributeDescriptions());

	PipelineCreateInfo ShaderTerrains::pipelineCreateInfo =
	{
		PIPELINE_POLYGON, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT // cullModeFlags
	};
}

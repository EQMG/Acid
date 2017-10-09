#include "ShaderSkyboxes.hpp"

#include "../models/Vertex.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	DescriptorType ShaderSkyboxes::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	DescriptorType ShaderSkyboxes::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL);
	//	DescriptorType ShaderSkyboxes::typeSamplerCubemap = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	Descriptor ShaderSkyboxes::descriptor = Descriptor::Create({ typeUboScene, typeUboObject }); // , typeSamplerCubemap
	InputState ShaderSkyboxes::inputState = InputState::Create(Vertex::GetBindingDescriptions(), Vertex::GetAttributeDescriptions());

	PipelineCreateInfo ShaderSkyboxes::pipelineCreateInfo =
	{
		PIPELINE_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_FRONT_BIT // cullModeFlags
	};
}

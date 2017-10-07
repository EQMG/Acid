#include "TestShader.hpp"

#include "../models/Vertex.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	DescriptorType TestShader::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	DescriptorType TestShader::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_VERTEX_BIT);
	DescriptorType TestShader::typeSamplerDiffuse = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	DescriptorType TestShader::typeSamplerSway = Texture::CreateDescriptor(3, VK_SHADER_STAGE_VERTEX_BIT);
	Descriptor TestShader::descriptor = Descriptor::Create({ typeUboScene, typeUboObject, typeSamplerDiffuse, typeSamplerSway });
	InputState TestShader::inputState = InputState::Create(Vertex::GetBindingDescriptions(), Vertex::GetAttributeDescriptions());

	PipelineCreateInfo TestShader::pipelineCreateInfo =
	{
		PIPELINE_POLYGON, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT // cullModeFlags
	};
}

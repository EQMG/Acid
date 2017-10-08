#include "ShaderTest.hpp"

#include "../models/Vertex.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	DescriptorType ShaderTest::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	DescriptorType ShaderTest::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_VERTEX_BIT);
	DescriptorType ShaderTest::typeSamplerDiffuse = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	DescriptorType ShaderTest::typeSamplerSway = Texture::CreateDescriptor(3, VK_SHADER_STAGE_VERTEX_BIT);
	Descriptor ShaderTest::descriptor = Descriptor::Create({ typeUboScene, typeUboObject, typeSamplerDiffuse, typeSamplerSway });
	InputState ShaderTest::inputState = InputState::Create(Vertex::GetBindingDescriptions(), Vertex::GetAttributeDescriptions());

	PipelineCreateInfo ShaderTest::pipelineCreateInfo =
	{
		PIPELINE_POLYGON, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT // cullModeFlags
	};
}

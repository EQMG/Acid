#include "SkyboxesShader.hpp"

#include "../models/Vertex.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	DescriptorType SkyboxesShader::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	DescriptorType SkyboxesShader::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_ALL);
//	DescriptorType SkyboxesShader::typeSamplerCubemap = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	Descriptor SkyboxesShader::descriptor = Descriptor::Create({ typeUboScene, typeUboObject }); // , typeSamplerCubemap

	InputState SkyboxesShader::inputState = InputState::Create(Vertex::GetBindingDescriptions(), Vertex::GetAttributeDescriptions());
}

#include "TestShader.hpp"

#include "../models/Vertex.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	DescriptorType TestShader::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_VERTEX_BIT);
	DescriptorType TestShader::typeUboObject = UniformBuffer::CreateDescriptor(1, VK_SHADER_STAGE_VERTEX_BIT);
	DescriptorType TestShader::typeTextureDiffuse = Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT);
	DescriptorType TestShader::typeTextureSway = Texture::CreateDescriptor(3, VK_SHADER_STAGE_VERTEX_BIT);
	Descriptor TestShader::descriptor = Descriptor::Create({ typeUboScene, typeUboObject, typeTextureDiffuse, typeTextureSway });

	InputState TestShader::inputState = InputState::Create(Vertex::GetBindingDescriptions(), Vertex::GetAttributeDescriptions());
}

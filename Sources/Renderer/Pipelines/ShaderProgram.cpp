#include "ShaderProgram.hpp"

#include <algorithm>
#include <SPIRV/GlslangToSpv.h>
#include "Helpers/FormatString.hpp"
#include "Textures/Texture.hpp"
#include "Textures/Cubemap.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"

namespace fl
{
	ShaderProgram::ShaderProgram(const std::string &name) :
		m_name(name),
		m_uniforms(new std::vector<Uniform *>()),
		m_uniformBlocks(new std::vector<UniformBlock *>()),
		m_vertexAttributes(new std::vector<VertexAttribute *>()),
		m_descriptors(new std::vector<DescriptorType>()),
		m_attributeDescriptions(new std::vector<VkVertexInputAttributeDescription>()),
		m_notFoundNames(std::vector<std::string>())
	{
	}

	ShaderProgram::~ShaderProgram()
	{
		for (auto uniform : *m_uniforms)
		{
			delete uniform;
		}

		for (auto uniformBlock : *m_uniformBlocks)
		{
			delete uniformBlock;
		}

		for (auto vertexAttribute : *m_vertexAttributes)
		{
			delete vertexAttribute;
		}

		delete m_uniforms;
		delete m_uniformBlocks;
		delete m_vertexAttributes;

		delete m_descriptors;
		delete m_attributeDescriptions;
	}

	void ShaderProgram::LoadProgram(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag)
	{
		for (int i = program.getNumLiveUniformBlocks() - 1; i >= 0; i--)
		{
			LoadUniformBlock(program, stageFlag, i);
		}

		for (int i = 0; i < program.getNumLiveUniformVariables(); i++)
		{
			LoadUniform(program, stageFlag, i);
		}

		for (int i = 0; i < program.getNumLiveAttributes(); i++)
		{
			LoadVertexAttribute(program, stageFlag, i);
		}
	}


	bool ShaderProgram::ReportedNotFound(const std::string &name, const bool &reportIfFound)
	{

		if (std::find(m_notFoundNames.begin(), m_notFoundNames.end(), name) == m_notFoundNames.end())
		{
			if (reportIfFound)
			{
				m_notFoundNames.push_back(name);
			}

			return true;
		}

		return false;
	}

	void ShaderProgram::ProcessShader()
	{
		// Sort uniforms by binding.
		std::sort(m_uniforms->begin(), m_uniforms->end(),
			[](Uniform *l, Uniform *r)
			{
				return l->GetBinding() < r->GetBinding();
			});

		// Sort uniform blocks by binding.
		std::sort(m_uniformBlocks->begin(), m_uniformBlocks->end(),
			[](UniformBlock *l, UniformBlock *r)
			{
				return l->GetBinding() < r->GetBinding();
			});

		// Sort uniform block uniforms by offsets.
		for (auto uniformBlock : *m_uniformBlocks)
		{
			std::sort(uniformBlock->GetUniforms()->begin(), uniformBlock->GetUniforms()->end(),
				[](Uniform *l, Uniform *r)
				{
					return l->GetOffset() < r->GetOffset();
				});
		}

		// Sort attributes by location.
		std::sort(m_vertexAttributes->begin(), m_vertexAttributes->end(),
			[](VertexAttribute *l, VertexAttribute *r)
			{
				return l->GetLocation() < r->GetLocation();
			});

		// Process to descriptors.
		for (auto uniformBlock : *m_uniformBlocks)
		{
			m_descriptors->push_back(UniformBuffer::CreateDescriptor(static_cast<uint32_t>(uniformBlock->GetBinding()), uniformBlock->GetStageFlags()));
		}

		for (auto uniform : *m_uniforms)
		{
			switch (uniform->GetGlType())
			{
			case 0x8B5E:
			case 0x904D:
				m_descriptors->push_back(Texture::CreateDescriptor(static_cast<uint32_t>(uniform->GetBinding()), uniform->GetStageFlags()));
				break;
			case 0x8B60:
			case 0x904E:
				m_descriptors->push_back(Cubemap::CreateDescriptor(static_cast<uint32_t>(uniform->GetBinding()), uniform->GetStageFlags()));
				break;
			default:
				break;
			}
		}

		// Process attribute descriptions.
		uint32_t currentOffset = 4;

		for (auto vertexAttribute : *m_vertexAttributes)
		{
			VkVertexInputAttributeDescription attributeDescription = {};
			attributeDescription.binding = 0;
			attributeDescription.location = static_cast<uint32_t>(vertexAttribute->GetLocation());
			attributeDescription.format = GlTypeToVk(vertexAttribute->GetGlType());
			attributeDescription.offset = currentOffset;

			m_attributeDescriptions->push_back(attributeDescription);
			currentOffset += vertexAttribute->GetSize();
		}
	}

	VkFormat ShaderProgram::GlTypeToVk(const int &type)
	{
		switch (type)
		{
		case 0x1406:
			return VK_FORMAT_R32_SFLOAT;
		case 0x8B50:
			return VK_FORMAT_R32G32_SFLOAT;
		case 0x8B51:
			return VK_FORMAT_R32G32B32_SFLOAT;
		case 0x8B52:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		default:
			return VK_FORMAT_UNDEFINED;
		}
	}

	int ShaderProgram::GetDescriptorLocation(const std::string &descriptor)
	{
		for (auto uniform : *m_uniforms)
		{
			if (uniform->GetName() == descriptor)
			{
				return uniform->GetBinding();
			}
		}

		for (auto uniformBlock : *m_uniformBlocks)
		{
			if (uniformBlock->GetName() == descriptor)
			{
				return uniformBlock->GetBinding();
			}
		}

		return -1;
	}

	std::string ShaderProgram::InsertDefineBlock(const std::string &shaderCode, const std::string &blockCode)
	{
		// TODO: Needs a rework.
		std::string result = shaderCode;
		size_t foundIndex0 = result.find('\n', 0);
		size_t foundIndex1 = result.find('\n', foundIndex0 + 1);
		size_t foundIndex2 = result.find('\n', foundIndex1 + 1);
		result.insert(foundIndex2, blockCode);
		return result;
	}

	Uniform *ShaderProgram::GetUniform(const std::string &uniformName)
	{
		for (auto uniform : *m_uniforms)
		{
			if (uniform->GetName() == uniformName)
			{
				return uniform;
			}
		}

		return nullptr;
	}

	UniformBlock *ShaderProgram::GetUniformBlock(const std::string &blockName)
	{
		for (auto uniformBlock : *m_uniformBlocks)
		{
			if (uniformBlock->GetName() == blockName)
			{
				return uniformBlock;
			}
		}

		return nullptr;
	}

	VertexAttribute *ShaderProgram::GetVertexAttribute(const std::string &attributeName)
	{
		for (auto attribute : *m_vertexAttributes)
		{
			if (attribute->GetName() == attributeName)
			{
				return attribute;
			}
		}

		return nullptr;
	}

	VkShaderStageFlagBits ShaderProgram::GetShaderStage(const std::string &filename)
	{
		if (FormatString::Contains(filename, ".comp"))
		{
			return VK_SHADER_STAGE_COMPUTE_BIT;
		}
		else if (FormatString::Contains(filename, ".vert"))
		{
			return VK_SHADER_STAGE_VERTEX_BIT;
		}
		else if (FormatString::Contains(filename, ".tesc"))
		{
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		}
		else if (FormatString::Contains(filename, ".tese"))
		{
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		}
		else if (FormatString::Contains(filename, ".geom"))
		{
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		}
		else if (FormatString::Contains(filename, ".frag"))
		{
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		return VK_SHADER_STAGE_ALL;
	}

	std::string ShaderProgram::ToString() const
	{
		std::stringstream result;

		if (!m_vertexAttributes->empty())
		{
			result << "Vertex Attributes: \n";

			for (auto vertexAttribute : *m_vertexAttributes)
			{
				result << "  - " << vertexAttribute->ToString() << "\n";
			}
		}

		if (!m_uniforms->empty())
		{
			result << "Uniforms: \n";

			for (auto uniform : *m_uniforms)
			{
				result << "  - " << uniform->ToString() << "\n";
			}
		}

		if (!m_uniformBlocks->empty())
		{
			result << "Uniform Blocks: \n";

			for (auto uniformBlock : *m_uniformBlocks)
			{
				result << "  - " << uniformBlock->ToString() << " \n";

				for (auto uniform : *uniformBlock->GetUniforms())
				{
					result << "    - " << uniform->ToString() << " \n";
				}
			}
		}

		return result.str();
	}

	void ShaderProgram::LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i)
	{
		for (auto uniformBlock : *m_uniformBlocks)
		{
			if (uniformBlock->GetName() == program.getUniformBlockName(i))
			{
				uniformBlock->SetStageFlags(VK_SHADER_STAGE_ALL);
				return;
			}
		}

		m_uniformBlocks->push_back(new UniformBlock(program.getUniformBlockName(i), program.getUniformBlockBinding(i), program.getUniformBlockSize(i), stageFlag));
	}

	void ShaderProgram::LoadUniform(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i)
	{
		if (program.getUniformBinding(i) == -1)
		{
			auto splitName = FormatString::Split(program.getUniformName(i), ".");

			if (splitName.size() == 2)
			{
				for (auto uniformBlock : *m_uniformBlocks)
				{
					if (uniformBlock->GetName() == splitName.at(0))
					{
						uniformBlock->AddUniform(new Uniform(splitName.at(1), program.getUniformBinding(i), program.getUniformBufferOffset(i),
							sizeof(float) * program.getUniformTType(i)->computeNumComponents(), program.getUniformType(i), stageFlag));
						return;
					}
				}
			}
		}

		for (auto uniform : *m_uniforms)
		{
			if (uniform->GetName() == program.getUniformName(i))
			{
				uniform->SetStageFlags(VK_SHADER_STAGE_ALL);
				return;
			}
		}

		m_uniforms->push_back(new Uniform(program.getUniformName(i), program.getUniformBinding(i), program.getUniformBufferOffset(i), -1, program.getUniformType(i), stageFlag));
	}

	void ShaderProgram::LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i)
	{
		for (auto vertexAttribute : *m_vertexAttributes)
		{
			if (vertexAttribute->GetName() == program.getAttributeName(i))
			{
				return;
			}
		}

		m_vertexAttributes->push_back(new VertexAttribute(program.getAttributeName(i), program.getAttributeTType(i)->getQualifier().layoutLocation,
			sizeof(float) * program.getAttributeTType(i)->getVectorSize(), program.getAttributeType(i)));
	}
}

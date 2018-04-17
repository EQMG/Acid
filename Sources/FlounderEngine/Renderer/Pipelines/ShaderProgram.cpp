#include "ShaderProgram.hpp"

#include "Helpers/FormatString.hpp"
#include "Textures/Texture.hpp"
#include "Textures/Cubemap.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	ShaderProgram::ShaderProgram() :
		m_uniforms(new std::vector<Uniform *>()),
		m_uniformBlocks(new std::vector<UniformBlock *>()),
		m_vertexAttributes(new std::vector<VertexAttribute *>()),
		m_descriptors(new std::vector<DescriptorType>()),
		m_attributeDescriptions(new std::vector<VkVertexInputAttributeDescription>())
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

		for (int i = 0; i < program.getNumLiveAttributes(); i++) // TODO: Load in correct order.
		{
			LoadVertexAttribute(program, stageFlag, i);
		}
	}

	void ShaderProgram::LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i)
	{
		for (auto uniformBlock : *m_uniformBlocks)
		{
			if (uniformBlock->m_name == program.getUniformBlockName(i))
			{
				uniformBlock->m_stageFlags = VK_SHADER_STAGE_ALL;
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
					if (uniformBlock->m_name == splitName.at(0))
					{
						uniformBlock->AddUniform(new Uniform(splitName.at(1), program.getUniformBinding(i), program.getUniformBufferOffset(i), -1, program.getUniformType(i), stageFlag));
						return;
					}
				}
			}
		}

		for (auto uniform : *m_uniforms)
		{
			if (uniform->m_name == program.getUniformName(i))
			{
				uniform->m_stageFlags = VK_SHADER_STAGE_ALL;
				return;
			}
		}

		m_uniforms->push_back(new Uniform(program.getUniformName(i), program.getUniformBinding(i), program.getUniformBufferOffset(i), -1, program.getUniformType(i), stageFlag));
	}

	void ShaderProgram::LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i)
	{
		for (auto vertexAttribute : *m_vertexAttributes)
		{
			if (vertexAttribute->m_name == program.getAttributeName(i))
			{
				return;
			}
		}

		m_vertexAttributes->push_back(new VertexAttribute(program.getAttributeName(i), program.getAttributeTType(i)->getQualifier().layoutLocation,
			sizeof(float) * program.getAttributeTType(i)->getVectorSize(), program.getAttributeType(i))); // program.getAttributeLocation(i), program.getAttributeSize(i)
	}

	void ShaderProgram::ProcessShader()
	{
		// Sort uniforms by binding.
		std::sort(m_uniforms->begin(), m_uniforms->end(),
			[](Uniform *l, Uniform *r)
			{
				return l->m_binding < r->m_binding;
			});

		// Sort uniform blocks by binding.
		std::sort(m_uniformBlocks->begin(), m_uniformBlocks->end(),
			[](UniformBlock *l, UniformBlock *r)
			{
				return l->m_binding < r->m_binding;
			});

		// Sort attributes by location.
		std::sort(m_vertexAttributes->begin(), m_vertexAttributes->end(),
			[](VertexAttribute *l, VertexAttribute *r)
			{
				return l->m_location < r->m_location;
			});

		// Sort uniform block uniforms by offsets.
		for (auto uniformBlock : *m_uniformBlocks)
		{
			std::sort(uniformBlock->m_uniforms->begin(), uniformBlock->m_uniforms->end(),
				[](Uniform *l, Uniform *r)
				{
					return l->m_offset < r->m_offset;
				});

			for (unsigned int i = 0; i < uniformBlock->m_uniforms->size(); i++)
			{
				if (i == uniformBlock->m_uniforms->size() - 1)
				{
					uniformBlock->m_uniforms->at(i)->m_size = uniformBlock->m_size - uniformBlock->m_uniforms->at(i)->m_offset;
					continue;
				}

				uniformBlock->m_uniforms->at(i)->m_size = uniformBlock->m_uniforms->at(i + 1)->m_offset - uniformBlock->m_uniforms->at(i)->m_offset;
			}
		}

		// Process to descriptors.
		for (auto uniformBlock : *m_uniformBlocks)
		{
			m_descriptors->push_back(UniformBuffer::CreateDescriptor(static_cast<uint32_t>(uniformBlock->m_binding), uniformBlock->m_stageFlags));
		}

		for (auto uniform : *m_uniforms)
		{
			switch (uniform->m_glType)
			{
			case 0x8B5E:
			case 0x904D:
				m_descriptors->push_back(Texture::CreateDescriptor(static_cast<uint32_t>(uniform->m_binding), uniform->m_stageFlags));
				break;
			case 0x8B60:
			case 0x904E:
				m_descriptors->push_back(Cubemap::CreateDescriptor(static_cast<uint32_t>(uniform->m_binding), uniform->m_stageFlags));
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
			attributeDescription.location = static_cast<uint32_t>(vertexAttribute->m_location);
			attributeDescription.format = GlTypeToVk(vertexAttribute->m_glType);
			attributeDescription.offset = currentOffset;

			m_attributeDescriptions->push_back(attributeDescription);
			currentOffset += vertexAttribute->m_size;
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
			if (uniform->m_name == descriptor)
			{
				return uniform->m_binding;
			}
		}

		for (auto uniformBlock : *m_uniformBlocks)
		{
			if (uniformBlock->m_name == descriptor)
			{
				return uniformBlock->m_binding;
			}
		}

		return -1;
	}

	std::string ShaderProgram::InsertDefineBlock(const std::string &shaderCode, const std::string &blockCode)
	{
		// TODO: Rework.
		std::string result = shaderCode;
		size_t foundIndex0 = result.find('\n', 0);
		size_t foundIndex1 = result.find('\n', foundIndex0 + 1);
		size_t foundIndex2 = result.find('\n', foundIndex1 + 1);
		result.insert(foundIndex2, blockCode);
		return result;
	}

	UniformBlock *ShaderProgram::GetUniformBlock(const std::string &blockName)
	{
		for (auto uniformBlock : *m_uniformBlocks)
		{
			if (uniformBlock->m_name == blockName)
			{
				return uniformBlock;
			}
		}

		return nullptr;
	}

	Uniform *ShaderProgram::GetBlockUniform(const std::string &blockName, const std::string &uniformName)
	{
		for (auto uniformBlock : *m_uniformBlocks)
		{
			if (uniformBlock->m_name != blockName)
			{
				continue;
			}

			for (auto uniform : *uniformBlock->m_uniforms)
			{
				if (uniform->m_name == uniformName)
				{
					return uniform;
				}
			}
		}

		return nullptr;
	}

	VertexAttribute *ShaderProgram::GetVertexAttribute(const std::string &attributeName)
	{
		for (auto attribute : *m_vertexAttributes)
		{
			if (attribute->m_name == attributeName)
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

	EShLanguage ShaderProgram::GetEshLanguage(const VkShaderStageFlagBits &stageFlag)
	{
		switch (stageFlag)
		{
		case VK_SHADER_STAGE_COMPUTE_BIT:
			return EShLangCompute;
		case VK_SHADER_STAGE_VERTEX_BIT:
			return EShLangVertex;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
			return EShLangTessControl;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
			return EShLangTessEvaluation;
		case VK_SHADER_STAGE_GEOMETRY_BIT:
			return EShLangGeometry;
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			return EShLangFragment;
		default:
			return EShLangCount;
		}
	}

	TBuiltInResource ShaderProgram::GetResources()
	{
		TBuiltInResource resources = {};
		resources.maxLights = 32;
		resources.maxClipPlanes = 6;
		resources.maxTextureUnits = 32;
		resources.maxTextureCoords = 32;
		resources.maxVertexAttribs = 64;
		resources.maxVertexUniformComponents = 4096;
		resources.maxVaryingFloats = 64;
		resources.maxVertexTextureImageUnits = 32;
		resources.maxCombinedTextureImageUnits = 80;
		resources.maxTextureImageUnits = 32;
		resources.maxFragmentUniformComponents = 4096;
		resources.maxDrawBuffers = 32;
		resources.maxVertexUniformVectors = 128;
		resources.maxVaryingVectors = 8;
		resources.maxFragmentUniformVectors = 16;
		resources.maxVertexOutputVectors = 16;
		resources.maxFragmentInputVectors = 15;
		resources.minProgramTexelOffset = -8;
		resources.maxProgramTexelOffset = 7;
		resources.maxClipDistances = 8;
		resources.maxComputeWorkGroupCountX = 65535;
		resources.maxComputeWorkGroupCountY = 65535;
		resources.maxComputeWorkGroupCountZ = 65535;
		resources.maxComputeWorkGroupSizeX = 1024;
		resources.maxComputeWorkGroupSizeY = 1024;
		resources.maxComputeWorkGroupSizeZ = 64;
		resources.maxComputeUniformComponents = 1024;
		resources.maxComputeTextureImageUnits = 16;
		resources.maxComputeImageUniforms = 8;
		resources.maxComputeAtomicCounters = 8;
		resources.maxComputeAtomicCounterBuffers = 1;
		resources.maxVaryingComponents = 60;
		resources.maxVertexOutputComponents = 64;
		resources.maxGeometryInputComponents = 64;
		resources.maxGeometryOutputComponents = 128;
		resources.maxFragmentInputComponents = 128;
		resources.maxImageUnits = 8;
		resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		resources.maxCombinedShaderOutputResources = 8;
		resources.maxImageSamples = 0;
		resources.maxVertexImageUniforms = 0;
		resources.maxTessControlImageUniforms = 0;
		resources.maxTessEvaluationImageUniforms = 0;
		resources.maxGeometryImageUniforms = 0;
		resources.maxFragmentImageUniforms = 8;
		resources.maxCombinedImageUniforms = 8;
		resources.maxGeometryTextureImageUnits = 16;
		resources.maxGeometryOutputVertices = 256;
		resources.maxGeometryTotalOutputComponents = 1024;
		resources.maxGeometryUniformComponents = 1024;
		resources.maxGeometryVaryingComponents = 64;
		resources.maxTessControlInputComponents = 128;
		resources.maxTessControlOutputComponents = 128;
		resources.maxTessControlTextureImageUnits = 16;
		resources.maxTessControlUniformComponents = 1024;
		resources.maxTessControlTotalOutputComponents = 4096;
		resources.maxTessEvaluationInputComponents = 128;
		resources.maxTessEvaluationOutputComponents = 128;
		resources.maxTessEvaluationTextureImageUnits = 16;
		resources.maxTessEvaluationUniformComponents = 1024;
		resources.maxTessPatchComponents = 120;
		resources.maxPatchVertices = 32;
		resources.maxTessGenLevel = 64;
		resources.maxViewports = 16;
		resources.maxVertexAtomicCounters = 0;
		resources.maxTessControlAtomicCounters = 0;
		resources.maxTessEvaluationAtomicCounters = 0;
		resources.maxGeometryAtomicCounters = 0;
		resources.maxFragmentAtomicCounters = 8;
		resources.maxCombinedAtomicCounters = 8;
		resources.maxAtomicCounterBindings = 1;
		resources.maxVertexAtomicCounterBuffers = 0;
		resources.maxTessControlAtomicCounterBuffers = 0;
		resources.maxTessEvaluationAtomicCounterBuffers = 0;
		resources.maxGeometryAtomicCounterBuffers = 0;
		resources.maxFragmentAtomicCounterBuffers = 1;
		resources.maxCombinedAtomicCounterBuffers = 1;
		resources.maxAtomicCounterBufferSize = 16384;
		resources.maxTransformFeedbackBuffers = 4;
		resources.maxTransformFeedbackInterleavedComponents = 64;
		resources.maxCullDistances = 8;
		resources.maxCombinedClipAndCullDistances = 8;
		resources.maxSamples = 4;
		resources.limits.nonInductiveForLoops = true;
		resources.limits.whileLoops = true;
		resources.limits.doWhileLoops = true;
		resources.limits.generalUniformIndexing = true;
		resources.limits.generalAttributeMatrixVectorIndexing = true;
		resources.limits.generalVaryingIndexing = true;
		resources.limits.generalSamplerIndexing = true;
		resources.limits.generalVariableIndexing = true;
		resources.limits.generalConstantMatrixVectorIndexing = true;
		return resources;
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

				for (auto uniform : *uniformBlock->m_uniforms)
				{
					result << "    - " << uniform->ToString() << " \n";
				}
			}
		}

		return result.str();
	}
}

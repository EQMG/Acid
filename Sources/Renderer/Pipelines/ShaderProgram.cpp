#include "ShaderProgram.hpp"

#include <SPIRV/GlslangToSpv.h>
#include "Display/Display.hpp"
#include "Helpers/FileSystem.hpp"
#include "Helpers/String.hpp"
#include "Renderer/Buffers/StorageBuffer.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Textures/Cubemap.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
	ShaderProgram::ShaderProgram(const std::string &name) :
		m_name(name),
		m_uniforms(std::vector<std::unique_ptr<Uniform>>()),
		m_uniformBlocks(std::vector<std::unique_ptr<UniformBlock>>()),
		m_vertexAttributes(std::vector<std::unique_ptr<VertexAttribute>>()),
		m_descriptorSetLayouts(std::vector<VkDescriptorSetLayoutBinding>()),
		m_descriptorPools(std::vector<VkDescriptorPoolSize>()),
		m_descriptorTypes(std::vector<VkDescriptorType>()),
		m_attributeDescriptions(std::vector<VkVertexInputAttributeDescription>()),
		m_notFoundNames(std::vector<std::string>())
	{
	}

	bool ShaderProgram::ReportedNotFound(const std::string &name, const bool &reportIfFound)
	{
		if (std::find(m_notFoundNames.begin(), m_notFoundNames.end(), name) == m_notFoundNames.end())
		{
			if (reportIfFound)
			{
				m_notFoundNames.emplace_back(name);
			}

			return true;
		}

		return false;
	}

	void ShaderProgram::ProcessShader()
	{
		// Sort uniforms by binding.
		std::sort(m_uniforms.begin(), m_uniforms.end(),
			[](const std::unique_ptr<Uniform> &l, const std::unique_ptr<Uniform> &r)
		{
			return l->GetBinding() < r->GetBinding();
		});

		// Sort uniform blocks by binding.
		std::sort(m_uniformBlocks.begin(), m_uniformBlocks.end(),
			[](const std::unique_ptr<UniformBlock> &l, const std::unique_ptr<UniformBlock> &r)
		{
			return l->GetBinding() < r->GetBinding();
		});

		// Sort uniform block uniforms by offsets.
		for (auto &uniformBlock : m_uniformBlocks)
		{
			std::sort(uniformBlock->GetUniforms().begin(), uniformBlock->GetUniforms().end(),
				[](const std::unique_ptr<Uniform> &l, const std::unique_ptr<Uniform> &r)
			{
				return l->GetOffset() < r->GetOffset();
			});
		}

		// Sort attributes by location.
		std::sort(m_vertexAttributes.begin(), m_vertexAttributes.end(),
			[](const std::unique_ptr<VertexAttribute> &l, const std::unique_ptr<VertexAttribute> &r)
		{
			return l->GetLocation() < r->GetLocation();
		});

		std::map<VkDescriptorType, uint32_t> descriptorPoolCounts = {};

		// Process to descriptors.
		for (auto &uniformBlock : m_uniformBlocks)
		{
			VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;

			switch (uniformBlock->GetType())
			{
				case BLOCK_UNIFORM:
					descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					m_descriptorSetLayouts.emplace_back(UniformBuffer::GetDescriptorSetLayout(static_cast<uint32_t>(uniformBlock->GetBinding()),
						descriptorType, uniformBlock->GetStageFlags(), 1));
					break;
				case BLOCK_STORAGE:
					descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
					m_descriptorSetLayouts.emplace_back(StorageBuffer::GetDescriptorSetLayout(static_cast<uint32_t>(uniformBlock->GetBinding()),
						descriptorType, uniformBlock->GetStageFlags(), 1));
					break;
				case BLOCK_PUSH:
					// Push constants are described in the pipeline.
					break;
				default:
					break;
			}

			IncrementDescriptorPool(descriptorPoolCounts, descriptorType);
		}

		for (auto &uniform : m_uniforms)
		{
			VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;

			switch (uniform->GetGlType())
			{
			case 0x8B5E: // GL_SAMPLER_2D
			case 0x904D: // GL_IMAGE_2D
			case 0x9108: // GL_SAMPLER_2D_MULTISAMPLE
			case 0x9055: // GL_IMAGE_2D_MULTISAMPLE
				descriptorType = uniform->IsWriteOnly() ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				m_descriptorSetLayouts.emplace_back(Texture::GetDescriptorSetLayout(static_cast<uint32_t>(uniform->GetBinding()),
					descriptorType, uniform->GetStageFlags(), 1));
				break;
			case 0x8B60: // GL_SAMPLER_CUBE
			case 0x9050: // GL_IMAGE_CUBE
				descriptorType = uniform->IsWriteOnly() ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				m_descriptorSetLayouts.emplace_back(Cubemap::GetDescriptorSetLayout(static_cast<uint32_t>(uniform->GetBinding()),
					descriptorType, uniform->GetStageFlags(), 1));
				break;
			default:
				break;
			}

			IncrementDescriptorPool(descriptorPoolCounts, descriptorType);
		}

		for (auto &[type, descriptorCount] : descriptorPoolCounts)
		{
			VkDescriptorPoolSize descriptorPoolSize = {};
			descriptorPoolSize.type = type;
			descriptorPoolSize.descriptorCount = descriptorCount;
			m_descriptorPools.emplace_back(descriptorPoolSize);
		}

		// Sort descriptors by binding.
		std::sort(m_descriptorSetLayouts.begin(), m_descriptorSetLayouts.end(),
		[](const VkDescriptorSetLayoutBinding &l, const VkDescriptorSetLayoutBinding &r)
		{
		    return l.binding < r.binding;
		});

		// Gets the descriptor type for each descriptor.
		for (auto &descriptor : m_descriptorSetLayouts)
		{
			m_descriptorTypes.emplace_back(descriptor.descriptorType);
		}

		// Process attribute descriptions.
		uint32_t currentOffset = 4;

		for (auto &vertexAttribute : m_vertexAttributes)
		{
			VkVertexInputAttributeDescription attributeDescription = {};
			attributeDescription.location = static_cast<uint32_t>(vertexAttribute->GetLocation());
			attributeDescription.binding = 0;
			attributeDescription.format = GlTypeToVk(vertexAttribute->GetGlType());
			attributeDescription.offset = currentOffset;
			m_attributeDescriptions.emplace_back(attributeDescription);
			currentOffset += vertexAttribute->GetSize();
		}
	}

	VkFormat ShaderProgram::GlTypeToVk(const int32_t &type)
	{
		switch (type)
		{
		case 0x1406: // GL_FLOAT
			return VK_FORMAT_R32_SFLOAT;
		case 0x8B50: // GL_FLOAT_VEC2
			return VK_FORMAT_R32G32_SFLOAT;
		case 0x8B51: // GL_FLOAT_VEC3
			return VK_FORMAT_R32G32B32_SFLOAT;
		case 0x8B52: // GL_FLOAT_VEC4
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case 0x1404: // GL_INT
			return VK_FORMAT_R32_SINT;
		case 0x8B53: // GL_INT_VEC2
			return VK_FORMAT_R32G32_SINT;
		case 0x8B54: // GL_INT_VEC3
			return VK_FORMAT_R32G32B32_SINT;
		case 0x8B55: // GL_INT_VEC4
			return VK_FORMAT_R32G32B32A32_SINT;
		case 0x1405: // GL_UNSIGNED_INT
			return VK_FORMAT_R32_SINT;
		case 0x8DC6: // GL_UNSIGNED_INT_VEC2
			return VK_FORMAT_R32G32_SINT;
		case 0x8DC7: // GL_UNSIGNED_INT_VEC3
			return VK_FORMAT_R32G32B32_SINT;
		case 0x8DC8: // GL_UNSIGNED_INT_VEC4
			return VK_FORMAT_R32G32B32A32_SINT;
		default:
			return VK_FORMAT_UNDEFINED;
		}
	}

	int32_t ShaderProgram::GetDescriptorLocation(const std::string &descriptor)
	{
		for (auto &uniform : m_uniforms)
		{
			if (uniform->GetName() == descriptor)
			{
				return uniform->GetBinding();
			}
		}

		for (auto &uniformBlock : m_uniformBlocks)
		{
			if (uniformBlock->GetName() == descriptor)
			{
				return uniformBlock->GetBinding();
			}
		}

		return -1;
	}

	std::optional<uint32_t> ShaderProgram::GetDescriptorSize(const std::string &descriptor)
	{
		for (auto &uniform : m_uniforms)
		{
			if (uniform->GetName() == descriptor)
			{
				return uniform->GetSize();
			}
		}

		for (auto &uniformBlock : m_uniformBlocks)
		{
			if (uniformBlock->GetName() == descriptor)
			{
				return uniformBlock->GetSize();
			}
		}

		return {};
	}

	uint32_t ShaderProgram::GetLastDescriptorBinding() const
	{
		uint32_t binding = 0;

		for (auto &descriptor : m_descriptorSetLayouts)
		{
			if (descriptor.binding > binding)
			{
				binding = descriptor.binding;
			}
		}

		return binding;
	}

	VkShaderStageFlagBits ShaderProgram::GetShaderStage(const std::string &filename)
	{
		std::string fileExt = String::Lowercase(FileSystem::FileSuffix(filename));

		if (fileExt == ".comp")
		{
			return VK_SHADER_STAGE_COMPUTE_BIT;
		}
		else if (fileExt == ".vert")
		{
			return VK_SHADER_STAGE_VERTEX_BIT;
		}
		else if (fileExt == ".tesc")
		{
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		}
		else if (fileExt == ".tese")
		{
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		}
		else if (fileExt == ".geom")
		{
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		}
		else if (fileExt == ".frag")
		{
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		return VK_SHADER_STAGE_ALL;
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

	std::string ShaderProgram::ProcessIncludes(const std::string &shaderCode)
	{
		auto lines = String::Split(shaderCode, "\n", true);

		std::stringstream result;

		for (auto &line : lines)
		{
			if (String::Contains(line, "#include"))
			{
				std::string filename = String::ReplaceFirst(line, "#include", "");
				filename = String::RemoveAll(filename, '\"');
				filename = String::Trim(filename);

				auto fileLoaded = Files::Read(filename);

				if (!fileLoaded)
				{
					Log::Error("Shader Include could not be loaded: '%s'\n", filename.c_str());
					continue;
				}

				result << "\n" << *fileLoaded << "\n";
				continue;
			}

			result << line << "\n";
		}

		return result.str();
	}

	EShLanguage GetEshLanguage(const VkShaderStageFlags &stageFlag)
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

	TBuiltInResource GetResources()
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

	VkShaderModule ShaderProgram::ProcessShader(const std::string &shaderCode, const VkShaderStageFlags &stageFlag)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		// Starts converting GLSL to SPIR-V.
		EShLanguage language = GetEshLanguage(stageFlag);
		glslang::TProgram program;
		glslang::TShader shader(language);
		TBuiltInResource resources = GetResources();

		// Enable SPIR-V and Vulkan rules when parsing GLSL.
		EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);
#if defined(ACID_VERBOSE)
		messages = (EShMessages)(messages | EShMsgDebugInfo);
#endif

		const char *shaderSource = shaderCode.c_str();
		shader.setStrings(&shaderSource, 1);

		shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 110);
		shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_1);
		shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_3);

		const int defaultVersion = glslang::EShTargetOpenGL_450;

		if (!shader.parse(&resources, defaultVersion, false, messages))
		{
			Log::Out("%s\n", shader.getInfoLog());
			Log::Out("%s\n", shader.getInfoDebugLog());
			Log::Error("SPRIV shader compile failed!\n");
		}

		program.addShader(&shader);

		if (!program.link(messages) || !program.mapIO())
		{
			Log::Error("Error while linking shader program.\n");
		}

		program.buildReflection();
	//	program.dumpReflection();
		LoadProgram(program, stageFlag);

		glslang::SpvOptions spvOptions;
#if defined(ACID_VERBOSE)
		spvOptions.generateDebugInfo = true;
		spvOptions.disableOptimizer = true;
		spvOptions.optimizeSize = false;
#else
		spvOptions.generateDebugInfo = false;
		spvOptions.disableOptimizer = false;
		spvOptions.optimizeSize = true;
#endif

		spv::SpvBuildLogger logger;
		std::vector<uint32_t> spirv;
		glslang::GlslangToSpv(*program.getIntermediate((EShLanguage)language), spirv, &logger, &spvOptions);

		VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = spirv.size() * sizeof(uint32_t);
		shaderModuleCreateInfo.pCode = spirv.data();
		VkShaderModule shaderModule = VK_NULL_HANDLE;
		Display::CheckVk(vkCreateShaderModule(logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule));
		return shaderModule;
	}

	Uniform *ShaderProgram::GetUniform(const std::string &uniformName)
	{
		for (auto &uniform : m_uniforms)
		{
			if (uniform->GetName() == uniformName)
			{
				return uniform.get();
			}
		}

		return nullptr;
	}

	UniformBlock *ShaderProgram::GetUniformBlock(const std::string &blockName)
	{
		for (auto &uniformBlock : m_uniformBlocks)
		{
			if (uniformBlock->GetName() == blockName)
			{
				return uniformBlock.get();
			}
		}

		return nullptr;
	}

	VertexAttribute *ShaderProgram::GetVertexAttribute(const std::string &attributeName)
	{
		for (auto &attribute : m_vertexAttributes)
		{
			if (attribute->GetName() == attributeName)
			{
				return attribute.get();
			}
		}

		return nullptr;
	}

	std::string ShaderProgram::ToString() const
	{
		std::stringstream result;

		if (!m_vertexAttributes.empty())
		{
			result << "Vertex Attributes: \n";

			for (auto &vertexAttribute : m_vertexAttributes)
			{
				result << "  - " << vertexAttribute->ToString() << "\n";
			}
		}

		if (!m_uniforms.empty())
		{
			result << "Uniforms: \n";

			for (auto &uniform : m_uniforms)
			{
				result << "  - " << uniform->ToString() << "\n";
			}
		}

		if (!m_uniformBlocks.empty())
		{
			result << "Uniform Blocks: \n";

			for (auto &uniformBlock : m_uniformBlocks)
			{
				result << "  - " << uniformBlock->ToString() << " \n";

				for (auto &uniform : uniformBlock->GetUniforms())
				{
					result << "	- " << uniform->ToString() << " \n";
				}
			}
		}

		return result.str();
	}

	void ShaderProgram::IncrementDescriptorPool(std::map<VkDescriptorType, uint32_t> &descriptorPoolCounts, const VkDescriptorType &type)
	{
		if (type == VK_DESCRIPTOR_TYPE_MAX_ENUM)
		{
			return;
		}

		auto it = descriptorPoolCounts.find(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

		if (it != descriptorPoolCounts.end())
		{
			it->second++;
		}
		else
		{
			descriptorPoolCounts.emplace(type, 1);
		}
	}

	void ShaderProgram::LoadProgram(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag)
	{
		for (int32_t i = program.getNumLiveUniformBlocks() - 1; i >= 0; i--)
		{
			LoadUniformBlock(program, stageFlag, i);
		}

		for (int32_t i = 0; i < program.getNumLiveUniformVariables(); i++)
		{
			LoadUniform(program, stageFlag, i);
		}

		for (int32_t i = 0; i < program.getNumLiveAttributes(); i++)
		{
			LoadVertexAttribute(program, stageFlag, i);
		}
	}

	void ShaderProgram::LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i)
	{
		for (auto &uniformBlock : m_uniformBlocks)
		{
			if (uniformBlock->GetName() == program.getUniformBlockName(i))
			{
				uniformBlock->SetStageFlags(uniformBlock->GetStageFlags() | stageFlag);
				return;
			}
		}

		UniformBlockType type = BLOCK_UNIFORM;

		if (strcmp(program.getUniformBlockTType(i)->getStorageQualifierString(), "buffer") == 0)
		{
			type = BLOCK_STORAGE;
		}

		if (program.getUniformBlockTType(i)->getQualifier().layoutPushConstant)
		{
			type = BLOCK_PUSH;
		}

		m_uniformBlocks.emplace_back(std::make_unique<UniformBlock>(program.getUniformBlockName(i), program.getUniformBlockBinding(i), program.getUniformBlockSize(i), stageFlag, type));
	}

	void ShaderProgram::LoadUniform(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i)
	{
		if (program.getUniformBinding(i) == -1)
		{
			auto splitName = String::Split(program.getUniformName(i), ".");

			if (splitName.size() == 2)
			{
				for (auto &uniformBlock : m_uniformBlocks)
				{
					if (uniformBlock->GetName() == splitName.at(0))
					{
						uniformBlock->AddUniform(new Uniform(splitName.at(1), program.getUniformBinding(i), program.getUniformBufferOffset(i),
							ComputeSize(program.getUniformTType(i)), program.getUniformType(i), false, false, stageFlag));
						return;
					}
				}
			}
		}

		for (auto &uniform : m_uniforms)
		{
			if (uniform->GetName() == program.getUniformName(i))
			{
				uniform->SetStageFlags(uniform->GetStageFlags() | stageFlag);
				return;
			}
		}

		auto &qualifier = program.getUniformTType(i)->getQualifier();
		m_uniforms.emplace_back(std::make_unique<Uniform>(program.getUniformName(i), program.getUniformBinding(i), program.getUniformBufferOffset(i), -1, program.getUniformType(i),
			qualifier.readonly, qualifier.writeonly, stageFlag));
	}

	void ShaderProgram::LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i)
	{
		for (auto &vertexAttribute : m_vertexAttributes)
		{
			if (vertexAttribute->GetName() == program.getAttributeName(i))
			{
				return;
			}
		}

		auto &qualifier = program.getAttributeTType(i)->getQualifier();
		m_vertexAttributes.emplace_back(std::make_unique<VertexAttribute>(program.getAttributeName(i), qualifier.layoutSet,
			qualifier.layoutLocation, ComputeSize(program.getAttributeTType(i)), program.getAttributeType(i)));
	}

	int32_t ShaderProgram::ComputeSize(const glslang::TType *ttype)
	{
		// glslang::TType::computeNumComponents is available but has many issues resolved in this method.
		int components = 0;

		if (ttype->getBasicType() == glslang::EbtStruct || ttype->getBasicType() == glslang::EbtBlock)
		{
			for (const auto &tl : *ttype->getStruct())
			{
				components += ComputeSize(tl.type);
			}
		}
		else if (ttype->getMatrixCols() != 0)
		{
			components = ttype->getMatrixCols() * ttype->getMatrixRows();
		}
		else
		{
			components = ttype->getVectorSize();
		}

		if (ttype->getArraySizes() != nullptr)
		{
			int32_t arraySize = 1;

			for (int d = 0; d < ttype->getArraySizes()->getNumDims(); ++d)
			{
				auto dimSize = ttype->getArraySizes()->getDimSize(d);

				// this only makes sense in paths that have a known array size
				if (dimSize != glslang::UnsizedArraySize)
				{
					arraySize *= dimSize;
				}
			}

			components *= arraySize;
		}

		return sizeof(float) * components;
	}
}

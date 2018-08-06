#include "ShaderProgram.hpp"

#include <SPIRV/GlslangToSpv.h>
#include "Display/Display.hpp"
#include "Helpers/FileSystem.hpp"
#include "Helpers/FormatString.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Textures/Cubemap.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
	ShaderProgram::ShaderProgram(const std::string &name) :
		m_name(name),
		m_uniforms(std::vector<Uniform *>()),
		m_uniformBlocks(std::vector<UniformBlock *>()),
		m_vertexAttributes(std::vector<VertexAttribute *>()),
		m_descriptors(std::vector<DescriptorType>()),
		m_attributeDescriptions(std::vector<VkVertexInputAttributeDescription>()),
		m_notFoundNames(std::vector<std::string>())
	{
	}

	ShaderProgram::~ShaderProgram()
	{
		for (auto &uniform : m_uniforms)
		{
			delete uniform;
		}

		for (auto &uniformBlock : m_uniformBlocks)
		{
			delete uniformBlock;
		}

		for (auto &vertexAttribute : m_vertexAttributes)
		{
			delete vertexAttribute;
		}
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
			[](Uniform *l, Uniform *r)
			{
				return l->GetBinding() < r->GetBinding();
			});

		// Sort uniform blocks by binding.
		std::sort(m_uniformBlocks.begin(), m_uniformBlocks.end(),
			[](UniformBlock *l, UniformBlock *r)
			{
				return l->GetBinding() < r->GetBinding();
			});

		// Sort uniform block uniforms by offsets.
		for (auto &uniformBlock : m_uniformBlocks)
		{
			std::sort(uniformBlock->GetUniforms()->begin(), uniformBlock->GetUniforms()->end(),
				[](Uniform *l, Uniform *r)
				{
					return l->GetOffset() < r->GetOffset();
				});
		}

		// Sort attributes by location.
		std::sort(m_vertexAttributes.begin(), m_vertexAttributes.end(),
			[](VertexAttribute *l, VertexAttribute *r)
			{
				return l->GetLocation() < r->GetLocation();
			});

		// Process to descriptors.
		for (auto &uniformBlock : m_uniformBlocks)
		{
			m_descriptors.emplace_back(UniformBuffer::CreateDescriptor(static_cast<uint32_t>(uniformBlock->GetBinding()), uniformBlock->GetStageFlags()));
		}

		for (auto &uniform : m_uniforms)
		{
			switch (uniform->GetGlType())
			{
			case 0x8B5E:
			case 0x904D:
				m_descriptors.emplace_back(Texture::CreateDescriptor(static_cast<uint32_t>(uniform->GetBinding()), uniform->GetStageFlags()));
				break;
			case 0x8B60:
			case 0x904E:
				m_descriptors.emplace_back(Cubemap::CreateDescriptor(static_cast<uint32_t>(uniform->GetBinding()), uniform->GetStageFlags()));
				break;
			default:
				break;
			}
		}

		// Process attribute descriptions.
		uint32_t currentOffset = 4;

		for (auto &vertexAttribute : m_vertexAttributes)
		{
			VkVertexInputAttributeDescription attributeDescription = {};
			attributeDescription.binding = 0;
			attributeDescription.location = static_cast<uint32_t>(vertexAttribute->GetLocation());
			attributeDescription.format = GlTypeToVk(vertexAttribute->GetGlType());
			attributeDescription.offset = currentOffset;

			m_attributeDescriptions.emplace_back(attributeDescription);
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

	uint32_t ShaderProgram::GetLastDescriptorBinding() const
	{
		uint32_t binding = 0;

		for (auto &descriptor : m_descriptors)
		{
			if (descriptor.GetBinding() > binding)
			{
				binding = descriptor.GetBinding();
			}
		}

		return binding;
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
		auto lines = FormatString::Split(shaderCode, "\n", true);

		std::stringstream result;

		for (auto &line : lines)
		{
			if (FormatString::Contains(line, "#include"))
			{
				std::string path = FormatString::Replace(line, "#include", "");
				path = FormatString::RemoveAll(path, '\"');
				path = FormatString::Trim(path);
				auto included = FileSystem::ReadTextFile(Files::SearchFile(path));

				if (included.has_value())
				{
					result << "\n" << included.value() << "\n";
				}

				continue;
			}

			result << line << "\n";
		}

		return result.str();
	}


	EShLanguage GetEshLanguage(const VkShaderStageFlagBits &stageFlag)
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

	VkShaderModule ShaderProgram::ProcessShader(const std::string &shaderCode, const VkShaderStageFlagBits &stageFlag)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		EShLanguage language = GetEshLanguage(stageFlag);

		// Starts converting GLSL to SPIR-V.
		glslang::TShader shader = glslang::TShader(language);
		glslang::TProgram program;
		const char *shaderStrings[1];
		TBuiltInResource resources = GetResources();

		// Enable SPIR-V and Vulkan rules when parsing GLSL.
		EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);

		shaderStrings[0] = shaderCode.c_str();
		shader.setStrings(shaderStrings, 1);

		shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 100);
		shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
		shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

	//	if (shader.preprocess(&resources, 100, ENoProfile, false, false, messages, &str, includer))
	//	{
	//		fprintf(stderr, "SPRIV shader preprocess failed!\n");
	//	}

		if (!shader.parse(&resources, 100, false, messages))
		{
			fprintf(stdout, "%s\n", shader.getInfoLog());
			fprintf(stdout, "%s\n", shader.getInfoDebugLog());
			fprintf(stderr, "SPRIV shader compile failed!\n");
		}

		program.addShader(&shader);

		if (!program.link(messages) || !program.mapIO())
		{
			fprintf(stderr, "Error while linking shader program.\n");
		}

		program.buildReflection();
	//	program.dumpReflection();
		LoadProgram(program, stageFlag);

		glslang::SpvOptions spvOptions;
		spvOptions.generateDebugInfo = true;
		spvOptions.disableOptimizer = true;
		spvOptions.optimizeSize = false;

		std::vector<uint32_t> spirv = std::vector<uint32_t>();
		glslang::GlslangToSpv(*program.getIntermediate(language), spirv, &spvOptions);

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
				return uniform;
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
				return uniformBlock;
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
				return attribute;
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

				for (auto &uniform : *uniformBlock->GetUniforms())
				{
					result << "    - " << uniform->ToString() << " \n";
				}
			}
		}

		return result.str();
	}

	void ShaderProgram::LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i)
	{
		for (auto &uniformBlock : m_uniformBlocks)
		{
			if (uniformBlock->GetName() == program.getUniformBlockName(i))
			{
				uniformBlock->SetStageFlags(VK_SHADER_STAGE_ALL);
				return;
			}
		}

		m_uniformBlocks.emplace_back(new UniformBlock(program.getUniformBlockName(i), program.getUniformBlockBinding(i), program.getUniformBlockSize(i), stageFlag));
	}

	void ShaderProgram::LoadUniform(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i)
	{
		if (program.getUniformBinding(i) == -1)
		{
			auto splitName = FormatString::Split(program.getUniformName(i), ".");

			if (splitName.size() == 2)
			{
				for (auto &uniformBlock : m_uniformBlocks)
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

		for (auto &uniform : m_uniforms)
		{
			if (uniform->GetName() == program.getUniformName(i))
			{
				uniform->SetStageFlags(VK_SHADER_STAGE_ALL);
				return;
			}
		}

		m_uniforms.emplace_back(new Uniform(program.getUniformName(i), program.getUniformBinding(i), program.getUniformBufferOffset(i), -1, program.getUniformType(i), stageFlag));
	}

	void ShaderProgram::LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i)
	{
		for (auto &vertexAttribute : m_vertexAttributes)
		{
			if (vertexAttribute->GetName() == program.getAttributeName(i))
			{
				return;
			}
		}

		m_vertexAttributes.emplace_back(new VertexAttribute(program.getAttributeName(i), program.getAttributeTType(i)->getQualifier().layoutLocation,
			sizeof(float) * program.getAttributeTType(i)->getVectorSize(), program.getAttributeType(i)));
	}
}

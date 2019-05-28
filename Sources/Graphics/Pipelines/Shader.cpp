#include "Shader.hpp"

#include <SPIRV/GlslangToSpv.h>
#include <glslang/Public/ShaderLang.h>
#include "Graphics/Graphics.hpp"
#include "Files/FileSystem.hpp"
#include "Helpers/String.hpp"
#include "Graphics/Buffers/StorageBuffer.hpp"
#include "Graphics/Buffers/UniformBuffer.hpp"
#include "Graphics/Images/Image2d.hpp"
#include "Graphics/Images/ImageCube.hpp"

namespace acid
{
class ShaderIncluder :
	public glslang::TShader::Includer
{
public:
	IncludeResult *includeLocal(const char *headerName, const char *includerName, size_t inclusionDepth) override
	{
		auto directory = FileSystem::ParentDirectory(includerName);
		auto fileLoaded = Files::Read(directory + "/" + headerName);

		if (!fileLoaded)
		{
			Log::Error("Shader Include could not be loaded: '%s'\n", headerName);
			return nullptr;
		}

		auto content = new char[fileLoaded->size()];
		std::memcpy(content, fileLoaded->c_str(), fileLoaded->size());
		return new IncludeResult(headerName, content, fileLoaded->size(), content);
	}

	IncludeResult *includeSystem(const char *headerName, const char *includerName, size_t inclusionDepth) override
	{
		auto fileLoaded = Files::Read(headerName);

		if (!fileLoaded)
		{
			Log::Error("Shader Include could not be loaded: '%s'\n", headerName);
			return nullptr;
		}

		auto content = new char[fileLoaded->size()];
		std::memcpy(content, fileLoaded->c_str(), fileLoaded->size());
		return new IncludeResult(headerName, content, fileLoaded->size(), content);
	}

	void releaseInclude(IncludeResult *result) override
	{
		if (result != nullptr)
		{
			delete[] static_cast<char *>(result->userData);
			delete result;
		}
	}
};

Shader::Shader() :
	m_lastDescriptorBinding(0)
{
}

bool Shader::ReportedNotFound(const std::string &name, const bool &reportIfFound) const
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

const Metadata &operator>>(const Metadata &metadata, Shader &shader)
{
	metadata.GetChild("Stages", shader.m_stages);
	metadata.GetChild("Uniforms", shader.m_uniforms);
	metadata.GetChild("Uniform Blocks", shader.m_uniformBlocks);
	metadata.GetChild("Attributes", shader.m_attributes);
	metadata.GetChild("Constants", shader.m_constants);
	//metadata.GetChild("Local Sizes", shader.m_localSizes);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Shader &shader)
{
	metadata.SetChild("Stages", shader.m_stages);
	metadata.SetChild("Uniforms", shader.m_uniforms);
	metadata.SetChild("Uniform Blocks", shader.m_uniformBlocks);
	metadata.SetChild("Constants", shader.m_constants);
	//metadata.SetChild("Local Sizes", shader.m_localSizes);
	return metadata;
}

VkFormat Shader::GlTypeToVk(const int32_t &type)
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

std::optional<uint32_t> Shader::GetDescriptorLocation(const std::string &name) const
{
	auto it = m_descriptorLocations.find(name);

	if (it == m_descriptorLocations.end())
	{
		return std::nullopt;
	}

	return it->second;
}

std::optional<uint32_t> Shader::GetDescriptorSize(const std::string &name) const
{
	auto it = m_descriptorSizes.find(name);

	if (it == m_descriptorSizes.end())
	{
		return std::nullopt;
	}

	return it->second;
}

std::optional<Shader::Uniform> Shader::GetUniform(const std::string &name) const
{
	auto it = m_uniforms.find(name);

	if (it == m_uniforms.end())
	{
		return std::nullopt;
	}

	return it->second;
}

std::optional<Shader::UniformBlock> Shader::GetUniformBlock(const std::string &name) const
{
	auto it = m_uniformBlocks.find(name);

	if (it == m_uniformBlocks.end())
	{
		return std::nullopt;
	}

	return it->second;
}

std::optional<Shader::Attribute> Shader::GetAttribute(const std::string &name) const
{
	auto it = m_attributes.find(name);

	if (it == m_attributes.end())
	{
		return std::nullopt;
	}

	return it->second;
}

std::vector<VkPushConstantRange> Shader::GetPushConstantRanges() const
{
	std::vector<VkPushConstantRange> pushConstantRanges;
	uint32_t currentOffset = 0;

	for (const auto &[uniformBlockName, uniformBlock] : m_uniformBlocks)
	{
		if (uniformBlock.GetType() != Shader::UniformBlock::Type::Push)
		{
			continue;
		}

		VkPushConstantRange pushConstantRange = {};
		pushConstantRange.stageFlags = uniformBlock.GetStageFlags();
		pushConstantRange.offset = currentOffset;
		pushConstantRange.size = static_cast<uint32_t>(uniformBlock.GetSize());
		pushConstantRanges.emplace_back(pushConstantRange);
		currentOffset += pushConstantRange.size;
	}

	return pushConstantRanges;
}

std::optional<VkDescriptorType> Shader::GetDescriptorType(const uint32_t &location) const
{
	auto it = m_descriptorTypes.find(location);

	if (it == m_descriptorTypes.end())
	{
		return std::nullopt;
	}

	return it->second;
}

VkShaderStageFlagBits Shader::GetShaderStage(const std::string &filename)
{
	std::string fileExt = String::Lowercase(FileSystem::FileSuffix(filename));

	if (fileExt == ".comp")
	{
		return VK_SHADER_STAGE_COMPUTE_BIT;
	}
	if (fileExt == ".vert")
	{
		return VK_SHADER_STAGE_VERTEX_BIT;
	}
	if (fileExt == ".tesc")
	{
		return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	}
	if (fileExt == ".tese")
	{
		return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	}
	if (fileExt == ".geom")
	{
		return VK_SHADER_STAGE_GEOMETRY_BIT;
	}
	if (fileExt == ".frag")
	{
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	}

	return VK_SHADER_STAGE_ALL;
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

VkShaderModule Shader::CreateShaderModule(const std::string &moduleName, const std::string &moduleCode, const std::string &preamble, const VkShaderStageFlags &moduleFlag)
{
	auto logicalDevice = Graphics::Get()->GetLogicalDevice();

	m_stages.emplace_back(moduleName);

	// Starts converting GLSL to SPIR-V.
	auto language = GetEshLanguage(moduleFlag);
	glslang::TProgram program;
	glslang::TShader shader(language);
	auto resources = GetResources();

	// Enable SPIR-V and Vulkan rules when parsing GLSL.
	auto messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);
#if defined(ACID_VERBOSE)
	messages = static_cast<EShMessages>(messages | EShMsgDebugInfo);
#endif

	auto shaderName = moduleName.c_str();
	auto shaderSource = moduleCode.c_str();
	shader.setStringsWithLengthsAndNames(&shaderSource, nullptr, &shaderName, 1);
	shader.setPreamble(preamble.c_str());

	shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 110);
	shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_1);
	shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_3);

	ShaderIncluder includer;

	auto defaultVersion = glslang::EShTargetVulkan_1_1;

	std::string str;

	if (!shader.preprocess(&resources, defaultVersion, ENoProfile, false, false, messages, &str, includer))
	{
		Log::Out("%s\n", shader.getInfoLog());
		Log::Out("%s\n", shader.getInfoDebugLog());
		Log::Error("SPRIV shader preprocess failed!\n");
	}

	if (!shader.parse(&resources, defaultVersion, true, messages, includer))
	{
		Log::Out("%s\n", shader.getInfoLog());
		Log::Out("%s\n", shader.getInfoDebugLog());
		Log::Error("SPRIV shader parse failed!\n");
	}

	program.addShader(&shader);

	if (!program.link(messages) || !program.mapIO())
	{
		Log::Error("Error while linking shader program.\n");
	}

	program.buildReflection();
	//program.dumpReflection();

	for (uint32_t dim = 0; dim < 3; ++dim)
	{
		auto localSize = program.getLocalSize(dim);

		if (localSize > 1)
		{
			m_localSizes[dim] = localSize;
		}
	}

	for (int32_t i = program.getNumLiveUniformBlocks() - 1; i >= 0; i--)
	{
		LoadUniformBlock(program, moduleFlag, i);
	}

	for (int32_t i = 0; i < program.getNumLiveUniformVariables(); i++)
	{
		LoadUniform(program, moduleFlag, i);
	}

	for (int32_t i = 0; i < program.getNumLiveAttributes(); i++)
	{
		LoadVertexAttribute(program, moduleFlag, i);
	}

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
	GlslangToSpv(*program.getIntermediate(static_cast<EShLanguage>(language)), spirv, &logger, &spvOptions);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = spirv.size() * sizeof(uint32_t);
	shaderModuleCreateInfo.pCode = spirv.data();

	VkShaderModule shaderModule;
	Graphics::CheckVk(vkCreateShaderModule(*logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule));
	return shaderModule;
}

void Shader::CreateReflection()
{
	std::map<VkDescriptorType, uint32_t> descriptorPoolCounts;

	// Process to descriptors.
	for (const auto &[uniformBlockName, uniformBlock] : m_uniformBlocks)
	{
		auto descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;

		switch (uniformBlock.m_type)
		{
		case UniformBlock::Type::Uniform:
			descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			m_descriptorSetLayouts.emplace_back(UniformBuffer::GetDescriptorSetLayout(static_cast<uint32_t>(uniformBlock.m_binding), descriptorType, uniformBlock.m_stageFlags, 1));
			break;
		case UniformBlock::Type::Storage:
			descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			m_descriptorSetLayouts.emplace_back(StorageBuffer::GetDescriptorSetLayout(static_cast<uint32_t>(uniformBlock.m_binding), descriptorType, uniformBlock.m_stageFlags, 1));
			break;
		case UniformBlock::Type::Push:
			// Push constants are described in the pipeline.
			break;
		default:
			break;
		}

		IncrementDescriptorPool(descriptorPoolCounts, descriptorType);
		m_descriptorLocations.emplace(uniformBlockName, uniformBlock.m_binding);
		m_descriptorSizes.emplace(uniformBlockName, uniformBlock.m_size);
	}

	for (const auto &[uniformName, uniform] : m_uniforms)
	{
		auto descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;

		switch (uniform.m_glType)
		{
		case 0x8B5E: // GL_SAMPLER_2D
		case 0x904D: // GL_IMAGE_2D
		case 0x9108: // GL_SAMPLER_2D_MULTISAMPLE
		case 0x9055: // GL_IMAGE_2D_MULTISAMPLE
			descriptorType = uniform.m_writeOnly ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			m_descriptorSetLayouts.emplace_back(Image2d::GetDescriptorSetLayout(static_cast<uint32_t>(uniform.m_binding), descriptorType, uniform.m_stageFlags, 1));
			break;
		case 0x8B60: // GL_SAMPLER_CUBE
		case 0x9050: // GL_IMAGE_CUBE
			descriptorType = uniform.m_writeOnly ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			m_descriptorSetLayouts.emplace_back(ImageCube::GetDescriptorSetLayout(static_cast<uint32_t>(uniform.m_binding), descriptorType, uniform.m_stageFlags, 1));
			break;
		default:
			break;
		}

		IncrementDescriptorPool(descriptorPoolCounts, descriptorType);
		m_descriptorLocations.emplace(uniformName, uniform.m_binding);
		m_descriptorSizes.emplace(uniformName, uniform.m_size);
	}

	for (const auto &[type, descriptorCount] : descriptorPoolCounts)
	{
		VkDescriptorPoolSize descriptorPoolSize = {};
		descriptorPoolSize.type = type;
		descriptorPoolSize.descriptorCount = descriptorCount;
		m_descriptorPools.emplace_back(descriptorPoolSize);
	}

	// TODO: This is a AMD workaround that works on NVidia too...
	m_descriptorPools.resize(6);
	m_descriptorPools[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	m_descriptorPools[0].descriptorCount = 4096;
	m_descriptorPools[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	m_descriptorPools[1].descriptorCount = 2048;
	m_descriptorPools[2].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	m_descriptorPools[2].descriptorCount = 2048;
	m_descriptorPools[3].type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
	m_descriptorPools[3].descriptorCount = 2048;
	m_descriptorPools[4].type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
	m_descriptorPools[4].descriptorCount = 2048;
	m_descriptorPools[5].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	m_descriptorPools[5].descriptorCount = 2048;

	// Sort descriptors by binding.
	std::sort(m_descriptorSetLayouts.begin(), m_descriptorSetLayouts.end(), [](const VkDescriptorSetLayoutBinding &l, const VkDescriptorSetLayoutBinding &r)
	{
		return l.binding < r.binding;
	});

	// Gets the last descriptors binding.
	if (!m_descriptorSetLayouts.empty())
	{
		m_lastDescriptorBinding = m_descriptorSetLayouts.back().binding;
	}

	// Gets the descriptor type for each descriptor.
	for (const auto &descriptor : m_descriptorSetLayouts)
	{
		m_descriptorTypes.emplace(descriptor.binding, descriptor.descriptorType);
	}

	// Process attribute descriptions.
	uint32_t currentOffset = 4;

	for (const auto &[attributeName, attribute] : m_attributes)
	{
		VkVertexInputAttributeDescription attributeDescription = {};
		attributeDescription.location = static_cast<uint32_t>(attribute.m_location);
		attributeDescription.binding = 0;
		attributeDescription.format = GlTypeToVk(attribute.m_glType);
		attributeDescription.offset = currentOffset;
		m_attributeDescriptions.emplace_back(attributeDescription);
		currentOffset += attribute.m_size;
	}
}

std::string Shader::ToString() const
{
	std::stringstream stream;

	if (!m_attributes.empty())
	{
		stream << "Vertex Attributes: \n";

		for (const auto &[attributeName, attribute] : m_attributes)
		{
			stream << "  - " << attributeName << ": " << attribute.ToString() << "\n";
		}
	}

	if (!m_uniforms.empty())
	{
		stream << "Uniforms: \n";

		for (const auto &[uniformName, uniform] : m_uniforms)
		{
			stream << "  - " << uniformName << ": " << uniform.ToString() << "\n";
		}
	}

	if (!m_uniformBlocks.empty())
	{
		stream << "Uniform Blocks: \n";

		for (const auto &[uniformBlockName, uniformBlock] : m_uniformBlocks)
		{
			stream << "  - " << uniformBlockName << ": " << uniformBlock.ToString() << " \n";

			for (const auto &[uniformName, uniform] : uniformBlock.GetUniforms())
			{
				stream << "	- " << uniformName << ": " << uniform.ToString() << " \n";
			}
		}
	}

	for (uint32_t dim = 0; dim < m_localSizes.size(); dim++)
	{
		static const std::string AXES[] = { "X", "Y", "Z" };

		if (m_localSizes[dim])
		{
			stream << "Local size " << AXES[dim] << ": " << *m_localSizes[dim] << " \n";
		}
	}

	return stream.str();
}

void Shader::IncrementDescriptorPool(std::map<VkDescriptorType, uint32_t> &descriptorPoolCounts, const VkDescriptorType &type)
{
	if (type == VK_DESCRIPTOR_TYPE_MAX_ENUM)
	{
		return;
	}

	auto it = descriptorPoolCounts.find(type);

	if (it != descriptorPoolCounts.end())
	{
		it->second++;
	}
	else
	{
		descriptorPoolCounts.emplace(type, 1);
	}
}

void Shader::LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i)
{
	auto reflection = program.getUniformBlock(i);

	for (auto &[uniformBlockName, uniformBlock] : m_uniformBlocks)
	{
		if (uniformBlockName == reflection.name)
		{
			uniformBlock.m_stageFlags |= stageFlag;
			return;
		}
	}

	auto type = UniformBlock::Type::None;

	if (reflection.getType()->getQualifier().storage == glslang::EvqUniform)
	{
		type = UniformBlock::Type::Uniform;
	}

	if (reflection.getType()->getQualifier().storage == glslang::EvqBuffer)
	{
		type = UniformBlock::Type::Storage;
	}

	if (reflection.getType()->getQualifier().layoutPushConstant)
	{
		type = UniformBlock::Type::Push;
	}

	m_uniformBlocks.emplace(reflection.name, UniformBlock(reflection.getBinding(), reflection.size, stageFlag, type));
}

void Shader::LoadUniform(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i)
{
	auto reflection = program.getUniform(i);

	if (reflection.getBinding() == -1)
	{
		auto splitName = String::Split(reflection.name, ".");

		if (splitName.size() > 1)
		{
			for (auto &[uniformBlockName, uniformBlock] : m_uniformBlocks)
			{
				if (uniformBlockName == splitName.at(0))
				{
					uniformBlock.m_uniforms.emplace(String::ReplaceFirst(reflection.name, splitName.at(0) + ".", ""),
						Uniform(reflection.getBinding(), reflection.offset, ComputeSize(reflection.getType()), reflection.glDefineType, false, false,
							stageFlag));
					return;
				}
			}
		}
	}

	for (auto &[uniformName, uniform] : m_uniforms)
	{
		if (uniformName == reflection.name)
		{
			uniform.m_stageFlags |= stageFlag;
			return;
		}
	}

	auto &qualifier = reflection.getType()->getQualifier();
	m_uniforms.emplace(reflection.name, Uniform(reflection.getBinding(), reflection.offset, -1, reflection.glDefineType, qualifier.readonly, qualifier.writeonly, stageFlag));
}

void Shader::LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i)
{
	auto reflection = program.getPipeInput(i);

	if (reflection.name.empty())
	{
		return;
	}

	for (const auto &[attributeName, attribute] : m_attributes)
	{
		if (attributeName == reflection.name)
		{
			return;
		}
	}

	auto &qualifier = reflection.getType()->getQualifier();
	m_attributes.emplace(reflection.name, Attribute(qualifier.layoutSet, qualifier.layoutLocation, ComputeSize(reflection.getType()), reflection.glDefineType));
}

int32_t Shader::ComputeSize(const glslang::TType *ttype)
{
	// TODO: glslang::TType::computeNumComponents is available but has many issues resolved in this method.
	int32_t components = 0;

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

		for (int32_t d = 0; d < ttype->getArraySizes()->getNumDims(); ++d)
		{
			auto dimSize = ttype->getArraySizes()->getDimSize(d);

			// This only makes sense in paths that have a known array size.
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

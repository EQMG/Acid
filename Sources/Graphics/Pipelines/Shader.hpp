#pragma once

#include <vulkan/vulkan.h>
#include "Serialized/Metadata.hpp"

namespace glslang
{
class TProgram;
class TType;
}

namespace acid
{
/**
 * @brief Class that loads and processes a shader, and provides a reflection.
 */
class ACID_EXPORT Shader
{
public:
	/**
	 * A define added to the start of a shader, first value is the define name and second is the value to be set.
	 */
	using Define = std::pair<std::string, std::string>;

	/**
	 * @brief Class used to define sets of vertex inputs used in a shader.
	 */
	class VertexInput
	{
	public:
		VertexInput(std::vector<VkVertexInputBindingDescription> bindingDescriptions = {}, std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {}) :
			m_binding(0),
			m_bindingDescriptions(std::move(bindingDescriptions)),
			m_attributeDescriptions(std::move(attributeDescriptions))
		{
		}

		friend const Metadata &operator>>(const Metadata &metadata, VertexInput &vertexInput)
		{
			metadata.GetChild("bindingDescriptions", vertexInput.m_bindingDescriptions);
			metadata.GetChild("attributeDescriptions", vertexInput.m_attributeDescriptions);
			return metadata;
		}

		friend Metadata &operator<<(Metadata &metadata, const VertexInput &vertexInput)
		{
			metadata.SetChild("bindingDescriptions", vertexInput.m_bindingDescriptions);
			metadata.SetChild("attributeDescriptions", vertexInput.m_attributeDescriptions);
			return metadata;
		}

		const std::vector<VkVertexInputBindingDescription> &GetBindingDescriptions() const { return m_bindingDescriptions; }

		const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return m_attributeDescriptions; }

		bool operator<(const VertexInput &other) const
		{
			return m_bindingDescriptions.front().binding < other.m_bindingDescriptions.front().binding;
		}

	private:
		uint32_t m_binding;
		std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
	};

	class Uniform
	{
	public:
		explicit Uniform(const int32_t &binding = -1, const int32_t &offset = -1, const int32_t &size = -1, const int32_t &glType = -1, const bool &readOnly = false,
			const bool &writeOnly = false, const VkShaderStageFlags &stageFlags = 0) :
			m_binding(binding),
			m_offset(offset),
			m_size(size),
			m_glType(glType),
			m_readOnly(readOnly),
			m_writeOnly(writeOnly),
			m_stageFlags(stageFlags)
		{
		}

		friend const Metadata &operator>>(const Metadata &metadata, Uniform &uniform)
		{
			metadata.GetChild("binding", uniform.m_binding);
			metadata.GetChild("offset", uniform.m_offset);
			metadata.GetChild("size", uniform.m_size);
			metadata.GetChild("glType", uniform.m_glType);
			metadata.GetChild("readOnly", uniform.m_readOnly);
			metadata.GetChild("writeOnly", uniform.m_writeOnly);
			metadata.GetChild("stageFlags", uniform.m_stageFlags);
			return metadata;
		}

		friend Metadata &operator<<(Metadata &metadata, const Uniform &uniform)
		{
			metadata.SetChild("binding", uniform.m_binding);
			metadata.SetChild("offset", uniform.m_offset);
			metadata.SetChild("size", uniform.m_size);
			metadata.SetChild("glType", uniform.m_glType);
			metadata.SetChild("readOnly", uniform.m_readOnly);
			metadata.SetChild("writeOnly", uniform.m_writeOnly);
			metadata.SetChild("stageFlags", uniform.m_stageFlags);
			return metadata;
		}

		const int32_t &GetBinding() const { return m_binding; }

		const int32_t &GetOffset() const { return m_offset; }

		const int32_t &GetSize() const { return m_size; }

		const int32_t &GetGlType() const { return m_glType; }

		const bool &IsReadOnly() const { return m_readOnly; }

		const bool &IsWriteOnly() const { return m_writeOnly; }

		const VkShaderStageFlags &GetStageFlags() const { return m_stageFlags; }

		bool operator==(const Uniform &other) const
		{
			return m_binding == other.m_binding && m_offset == other.m_offset && m_size == other.m_size && m_glType == other.m_glType && m_readOnly == other.m_readOnly
				&& m_writeOnly == other.m_writeOnly && m_stageFlags == other.m_stageFlags;
		}

		bool operator!=(const Uniform &other) const
		{
			return !(*this == other);
		}

		std::string ToString() const
		{
			std::stringstream stream;
			stream << "Uniform(binding " << m_binding << ", offset " << m_offset << ", size " << m_size << ", glType " << m_glType << ")";
			return stream.str();
		}

	private:
		friend class Shader;

		int32_t m_binding;
		int32_t m_offset;
		int32_t m_size;
		int32_t m_glType;
		bool m_readOnly;
		bool m_writeOnly;
		VkShaderStageFlags m_stageFlags;
	};

	class UniformBlock
	{
	public:
		enum class Type
		{
			None, Uniform, Storage, Push
		};

		explicit UniformBlock(const int32_t &binding = -1, const int32_t &size = -1, const VkShaderStageFlags &stageFlags = 0, const Type &type = Type::Uniform) :
			m_binding(binding),
			m_size(size),
			m_stageFlags(stageFlags),
			m_type(type)
		{
		}

		friend const Metadata &operator>>(const Metadata &metadata, UniformBlock &uniformBlock)
		{
			metadata.GetChild("binding", uniformBlock.m_binding);
			metadata.GetChild("size", uniformBlock.m_size);
			metadata.GetChild("stageFlags", uniformBlock.m_stageFlags);
			metadata.GetChild("type", uniformBlock.m_type);
			metadata.GetChild("uniforms", uniformBlock.m_uniforms);
			return metadata;
		}

		friend Metadata &operator<<(Metadata &metadata, const UniformBlock &uniformBlock)
		{
			metadata.SetChild("binding", uniformBlock.m_binding);
			metadata.SetChild("size", uniformBlock.m_size);
			metadata.SetChild("stageFlags", uniformBlock.m_stageFlags);
			metadata.SetChild("type", uniformBlock.m_type);
			metadata.SetChild("uniforms", uniformBlock.m_uniforms);
			return metadata;
		}

		const int32_t &GetBinding() const { return m_binding; }

		const int32_t &GetSize() const { return m_size; }

		const VkShaderStageFlags &GetStageFlags() const { return m_stageFlags; }

		const Type &GetType() const { return m_type; }

		const std::map<std::string, Uniform> &GetUniforms() const { return m_uniforms; }

		std::optional<Uniform> GetUniform(const std::string &name) const
		{
			auto it{m_uniforms.find(name)};

			if (it == m_uniforms.end())
			{
				return std::nullopt;
			}

			return it->second;
		}

		bool operator==(const UniformBlock &other) const
		{
			return m_binding == other.m_binding && m_size == other.m_size && m_stageFlags == other.m_stageFlags && m_type == other.m_type && m_uniforms == other.m_uniforms;
		}

		bool operator!=(const UniformBlock &other) const
		{
			return !(*this == other);
		}

		std::string ToString() const
		{
			std::stringstream stream;
			stream << "UniformBlock(binding " << m_binding << ", size " << m_size << ", type " << static_cast<uint32_t>(m_type) << ")";
			return stream.str();
		}

	private:
		friend class Shader;

		int32_t m_binding;
		int32_t m_size;
		VkShaderStageFlags m_stageFlags;
		Type m_type;
		std::map<std::string, Uniform> m_uniforms;
	};

	class Attribute
	{
	public:
		explicit Attribute(const int32_t &set = -1, const int32_t &location = -1, const int32_t &size = -1, const int32_t &glType = -1) :
			m_set(set),
			m_location(location),
			m_size(size),
			m_glType(glType)
		{
		}

		friend const Metadata &operator>>(const Metadata &metadata, Attribute &attribute)
		{
			metadata.GetChild("set", attribute.m_set);
			metadata.GetChild("location", attribute.m_location);
			metadata.GetChild("size", attribute.m_size);
			metadata.GetChild("glType", attribute.m_glType);
			return metadata;
		}

		friend Metadata &operator<<(Metadata &metadata, const Attribute &attribute)
		{
			metadata.SetChild("set", attribute.m_set);
			metadata.SetChild("location", attribute.m_location);
			metadata.SetChild("size", attribute.m_size);
			metadata.SetChild("glType", attribute.m_glType);
			return metadata;
		}

		const int32_t &GetSet() const { return m_set; }

		const int32_t &GetLocation() const { return m_location; }

		const int32_t &GetSize() const { return m_size; }

		const int32_t &GetGlType() const { return m_glType; }

		bool operator==(const Attribute &other) const
		{
			return m_set == other.m_set && m_location == other.m_location && m_size == other.m_size && m_glType == other.m_glType;
		}

		bool operator!=(const Attribute &other) const
		{
			return !(*this == other);
		}

		std::string ToString() const
		{
			std::stringstream stream;
			stream << "VertexAttribute(set " << m_set << "', location " << m_location << ", size " << m_size << ", glType " << m_glType << ")";
			return stream.str();
		}

	private:
		friend class Shader;

		int32_t m_set;
		int32_t m_location;
		int32_t m_size;
		int32_t m_glType;
	};

	class Constant
	{
	public:
		explicit Constant(const int32_t &binding = -1, const int32_t &size = -1, const VkShaderStageFlags &stageFlags = 0, const int32_t &glType = -1) :
			m_binding(binding),
			m_size(size),
			m_stageFlags(stageFlags),
			m_glType(glType)
		{
		}

		friend const Metadata &operator>>(const Metadata &metadata, Constant &constant)
		{
			metadata.GetChild("binding", constant.m_binding);
			metadata.GetChild("size", constant.m_size);
			metadata.GetChild("stageFlags", constant.m_stageFlags);
			metadata.GetChild("glType", constant.m_glType);
			return metadata;
		}

		friend Metadata &operator<<(Metadata &metadata, const Constant &constant)
		{
			metadata.SetChild("binding", constant.m_binding);
			metadata.SetChild("size", constant.m_size);
			metadata.SetChild("stageFlags", constant.m_stageFlags);
			metadata.SetChild("glType", constant.m_glType);
			return metadata;
		}

		const int32_t &GetBinding() const { return m_binding; }

		const int32_t &GetSize() const { return m_size; }

		const VkShaderStageFlags &GetStageFlags() const { return m_stageFlags; }

		const int32_t &GetGlType() const { return m_glType; }

		bool operator==(const Constant &other) const
		{
			return m_binding == other.m_binding && m_size == other.m_size && m_stageFlags == other.m_stageFlags && m_glType == other.m_glType;
		}

		bool operator!=(const Constant &other) const
		{
			return !(*this == other);
		}

		std::string ToString() const
		{
			std::stringstream stream;
			stream << "Constant(binding " << m_binding << "', size " << m_size << ", stageFlags " << m_stageFlags << ", glType " << m_glType << ")";
			return stream.str();
		}

	private:
		friend class Shader;

		int32_t m_binding;
		int32_t m_size;
		VkShaderStageFlags m_stageFlags;
		int32_t m_glType;
	};

	Shader();

	const std::filesystem::path &GetName() const { return m_stages.back(); }

	bool ReportedNotFound(const std::string &name, const bool &reportIfFound) const;

	friend const Metadata &operator>>(const Metadata &metadata, Shader &shader);

	friend Metadata &operator<<(Metadata &metadata, const Shader &shader);

	static VkFormat GlTypeToVk(const int32_t &type);

	std::optional<uint32_t> GetDescriptorLocation(const std::string &name) const;

	std::optional<uint32_t> GetDescriptorSize(const std::string &name) const;

	std::optional<Uniform> GetUniform(const std::string &name) const;

	std::optional<UniformBlock> GetUniformBlock(const std::string &name) const;

	std::optional<Attribute> GetAttribute(const std::string &name) const;

	std::vector<VkPushConstantRange> GetPushConstantRanges() const;

	const uint32_t &GetLastDescriptorBinding() const { return m_lastDescriptorBinding; }

	const std::map<std::string, Uniform> &GetUniforms() const { return m_uniforms; };

	const std::map<std::string, UniformBlock> &GetUniformBlocks() const { return m_uniformBlocks; };

	const std::map<std::string, Attribute> &GetAttributes() const { return m_attributes; };

	const std::map<std::string, Constant> &GetConstants() const { return m_constants; };

	const std::array<std::optional<uint32_t>, 3> &GetLocalSizes() const { return m_localSizes; }

	const std::vector<VkDescriptorSetLayoutBinding> &GetDescriptorSetLayouts() const { return m_descriptorSetLayouts; }

	const std::vector<VkDescriptorPoolSize> &GetDescriptorPools() const { return m_descriptorPools; }

	std::optional<VkDescriptorType> GetDescriptorType(const uint32_t &location) const;

	const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return m_attributeDescriptions; }

	static VkShaderStageFlagBits GetShaderStage(const std::filesystem::path &filename);

	VkShaderModule CreateShaderModule(const std::filesystem::path &moduleName, const std::string &moduleCode, const std::string &preamble, const VkShaderStageFlags &moduleFlag);

	void CreateReflection();

	std::string ToString() const;

private:
	static void IncrementDescriptorPool(std::map<VkDescriptorType, uint32_t> &descriptorPoolCounts, const VkDescriptorType &type);

	void LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

	void LoadUniform(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

	void LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

	static int32_t ComputeSize(const glslang::TType *ttype);

	std::vector<std::filesystem::path> m_stages;
	std::map<std::string, Uniform> m_uniforms;
	std::map<std::string, UniformBlock> m_uniformBlocks;
	std::map<std::string, Attribute> m_attributes;
	std::map<std::string, Constant> m_constants;

	std::array<std::optional<uint32_t>, 3> m_localSizes;

	std::map<std::string, uint32_t> m_descriptorLocations;
	std::map<std::string, uint32_t> m_descriptorSizes;

	std::vector<VkDescriptorSetLayoutBinding> m_descriptorSetLayouts;
	uint32_t m_lastDescriptorBinding{};
	std::vector<VkDescriptorPoolSize> m_descriptorPools;
	std::map<uint32_t, VkDescriptorType> m_descriptorTypes;
	std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;

	mutable std::vector<std::string> m_notFoundNames;
};

inline const Metadata &operator>>(const Metadata &metadata, VkVertexInputBindingDescription &bindingDescription)
{
	metadata.GetChild("binding", bindingDescription.binding);
	metadata.GetChild("stride", bindingDescription.stride);
	metadata.GetChild("inputRate", bindingDescription.inputRate);
	return metadata;
}

inline Metadata &operator<<(Metadata &metadata, const VkVertexInputBindingDescription &bindingDescription)
{
	metadata.SetChild("binding", bindingDescription.binding);
	metadata.SetChild("stride", bindingDescription.stride);
	metadata.SetChild("inputRate", bindingDescription.inputRate);
	return metadata;
}

inline const Metadata &operator>>(const Metadata &metadata, VkVertexInputAttributeDescription &attributeDescription)
{
	metadata.GetChild("location", attributeDescription.location);
	metadata.GetChild("binding", attributeDescription.binding);
	metadata.GetChild("format", attributeDescription.format);
	metadata.GetChild("offset", attributeDescription.offset);
	return metadata;
}

inline Metadata &operator<<(Metadata &metadata, const VkVertexInputAttributeDescription &attributeDescription)
{
	metadata.SetChild("location", attributeDescription.location);
	metadata.SetChild("binding", attributeDescription.binding);
	metadata.SetChild("format", attributeDescription.format);
	metadata.SetChild("offset", attributeDescription.offset);
	return metadata;
}
}

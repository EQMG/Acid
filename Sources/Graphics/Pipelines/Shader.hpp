#pragma once

#include <vulkan/vulkan.h>
#include "Serialized/Node.hpp"

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

		const std::vector<VkVertexInputBindingDescription> &GetBindingDescriptions() const { return m_bindingDescriptions; }

		const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return m_attributeDescriptions; }

		bool operator<(const VertexInput &other) const
		{
			return m_bindingDescriptions.front().binding < other.m_bindingDescriptions.front().binding;
		}

		friend const Node &operator>>(const Node &node, VertexInput &vertexInput)
		{
			node["bindingDescriptions"].Get(vertexInput.m_bindingDescriptions);
			node["attributeDescriptions"].Get(vertexInput.m_attributeDescriptions);
			return node;
		}

		friend Node &operator<<(Node &node, const VertexInput &vertexInput)
		{
			node["bindingDescriptions"].Set(vertexInput.m_bindingDescriptions);
			node["attributeDescriptions"].Set(vertexInput.m_attributeDescriptions);
			return node;
		}

	private:
		uint32_t m_binding;
		std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
	};

	class Uniform
	{
		friend class Shader;
	public:
		explicit Uniform(int32_t binding = -1, int32_t offset = -1, int32_t size = -1, int32_t glType = -1, bool readOnly = false,
			bool writeOnly = false, const VkShaderStageFlags &stageFlags = 0) :
			m_binding(binding),
			m_offset(offset),
			m_size(size),
			m_glType(glType),
			m_readOnly(readOnly),
			m_writeOnly(writeOnly),
			m_stageFlags(stageFlags)
		{
		}

		int32_t GetBinding() const { return m_binding; }

		int32_t GetOffset() const { return m_offset; }

		int32_t GetSize() const { return m_size; }

		int32_t GetGlType() const { return m_glType; }

		bool IsReadOnly() const { return m_readOnly; }

		bool IsWriteOnly() const { return m_writeOnly; }

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

		friend const Node &operator>>(const Node &node, Uniform &uniform)
		{
			node["binding"].Get(uniform.m_binding);
			node["offset"].Get(uniform.m_offset);
			node["size"].Get(uniform.m_size);
			node["glType"].Get(uniform.m_glType);
			node["readOnly"].Get(uniform.m_readOnly);
			node["writeOnly"].Get(uniform.m_writeOnly);
			node["stageFlags"].Get(uniform.m_stageFlags);
			return node;
		}

		friend Node &operator<<(Node &node, const Uniform &uniform)
		{
			node["binding"].Set(uniform.m_binding);
			node["offset"].Set(uniform.m_offset);
			node["size"].Set(uniform.m_size);
			node["glType"].Set(uniform.m_glType);
			node["readOnly"].Set(uniform.m_readOnly);
			node["writeOnly"].Set(uniform.m_writeOnly);
			node["stageFlags"].Set(uniform.m_stageFlags);
			return node;
		}

	private:
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
		friend class Shader;
	public:
		enum class Type
		{
			None, Uniform, Storage, Push
		};

		explicit UniformBlock(int32_t binding = -1, int32_t size = -1, const VkShaderStageFlags &stageFlags = 0, const Type &type = Type::Uniform) :
			m_binding(binding),
			m_size(size),
			m_stageFlags(stageFlags),
			m_type(type)
		{
		}

		int32_t GetBinding() const { return m_binding; }

		int32_t GetSize() const { return m_size; }

		const VkShaderStageFlags &GetStageFlags() const { return m_stageFlags; }

		const Type &GetType() const { return m_type; }

		const std::map<std::string, Uniform> &GetUniforms() const { return m_uniforms; }

		std::optional<Uniform> GetUniform(const std::string &name) const
		{
			auto it = m_uniforms.find(name);

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

		friend const Node &operator>>(const Node &node, UniformBlock &uniformBlock)
		{
			node["binding"].Get(uniformBlock.m_binding);
			node["size"].Get(uniformBlock.m_size);
			node["stageFlags"].Get(uniformBlock.m_stageFlags);
			node["type"].Get(uniformBlock.m_type);
			node["uniforms"].Get(uniformBlock.m_uniforms);
			return node;
		}

		friend Node &operator<<(Node &node, const UniformBlock &uniformBlock)
		{
			node["binding"].Set(uniformBlock.m_binding);
			node["size"].Set(uniformBlock.m_size);
			node["stageFlags"].Set(uniformBlock.m_stageFlags);
			node["type"].Set(uniformBlock.m_type);
			node["uniforms"].Set(uniformBlock.m_uniforms);
			return node;
		}

	private:
		int32_t m_binding;
		int32_t m_size;
		VkShaderStageFlags m_stageFlags;
		Type m_type;
		std::map<std::string, Uniform> m_uniforms;
	};

	class Attribute
	{
		friend class Shader;
	public:
		explicit Attribute(int32_t set = -1, int32_t location = -1, int32_t size = -1, int32_t glType = -1) :
			m_set(set),
			m_location(location),
			m_size(size),
			m_glType(glType)
		{
		}

		int32_t GetSet() const { return m_set; }

		int32_t GetLocation() const { return m_location; }

		int32_t GetSize() const { return m_size; }

		int32_t GetGlType() const { return m_glType; }

		bool operator==(const Attribute &other) const
		{
			return m_set == other.m_set && m_location == other.m_location && m_size == other.m_size && m_glType == other.m_glType;
		}

		bool operator!=(const Attribute &other) const
		{
			return !(*this == other);
		}

		friend const Node &operator>>(const Node &node, Attribute &attribute)
		{
			node["set"].Get(attribute.m_set);
			node["location"].Get(attribute.m_location);
			node["size"].Get(attribute.m_size);
			node["glType"].Get(attribute.m_glType);
			return node;
		}

		friend Node &operator<<(Node &node, const Attribute &attribute)
		{
			node["set"].Set(attribute.m_set);
			node["location"].Set(attribute.m_location);
			node["size"].Set(attribute.m_size);
			node["glType"].Set(attribute.m_glType);
			return node;
		}

	private:
		int32_t m_set;
		int32_t m_location;
		int32_t m_size;
		int32_t m_glType;
	};

	class Constant
	{
		friend class Shader;
	public:
		explicit Constant(int32_t binding = -1, int32_t size = -1, const VkShaderStageFlags &stageFlags = 0, int32_t glType = -1) :
			m_binding(binding),
			m_size(size),
			m_stageFlags(stageFlags),
			m_glType(glType)
		{
		}

		int32_t GetBinding() const { return m_binding; }

		int32_t GetSize() const { return m_size; }

		const VkShaderStageFlags &GetStageFlags() const { return m_stageFlags; }

		int32_t GetGlType() const { return m_glType; }

		bool operator==(const Constant &other) const
		{
			return m_binding == other.m_binding && m_size == other.m_size && m_stageFlags == other.m_stageFlags && m_glType == other.m_glType;
		}

		bool operator!=(const Constant &other) const
		{
			return !(*this == other);
		}

		friend const Node &operator>>(const Node &node, Constant &constant)
		{
			node["binding"].Get(constant.m_binding);
			node["size"].Get(constant.m_size);
			node["stageFlags"].Get(constant.m_stageFlags);
			node["glType"].Get(constant.m_glType);
			return node;
		}

		friend Node &operator<<(Node &node, const Constant &constant)
		{
			node["binding"].Set(constant.m_binding);
			node["size"].Set(constant.m_size);
			node["stageFlags"].Set(constant.m_stageFlags);
			node["glType"].Set(constant.m_glType);
			return node;
		}

	private:
		int32_t m_binding;
		int32_t m_size;
		VkShaderStageFlags m_stageFlags;
		int32_t m_glType;
	};

	Shader();

	const std::filesystem::path &GetName() const { return m_stages.back(); }

	bool ReportedNotFound(const std::string &name, bool reportIfFound) const;

	static VkFormat GlTypeToVk(int32_t type);

	std::optional<uint32_t> GetDescriptorLocation(const std::string &name) const;

	std::optional<uint32_t> GetDescriptorSize(const std::string &name) const;

	std::optional<Uniform> GetUniform(const std::string &name) const;

	std::optional<UniformBlock> GetUniformBlock(const std::string &name) const;

	std::optional<Attribute> GetAttribute(const std::string &name) const;

	std::vector<VkPushConstantRange> GetPushConstantRanges() const;

	uint32_t GetLastDescriptorBinding() const { return m_lastDescriptorBinding; }

	const std::map<std::string, Uniform> &GetUniforms() const { return m_uniforms; };

	const std::map<std::string, UniformBlock> &GetUniformBlocks() const { return m_uniformBlocks; };

	const std::map<std::string, Attribute> &GetAttributes() const { return m_attributes; };

	const std::map<std::string, Constant> &GetConstants() const { return m_constants; };

	const std::array<std::optional<uint32_t>, 3> &GetLocalSizes() const { return m_localSizes; }

	const std::vector<VkDescriptorSetLayoutBinding> &GetDescriptorSetLayouts() const { return m_descriptorSetLayouts; }

	const std::vector<VkDescriptorPoolSize> &GetDescriptorPools() const { return m_descriptorPools; }

	std::optional<VkDescriptorType> GetDescriptorType(uint32_t location) const;

	const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return m_attributeDescriptions; }

	static VkShaderStageFlagBits GetShaderStage(const std::filesystem::path &filename);

	VkShaderModule CreateShaderModule(const std::filesystem::path &moduleName, const std::string &moduleCode, const std::string &preamble, const VkShaderStageFlags &moduleFlag);

	void CreateReflection();

	friend const Node &operator>>(const Node &node, Shader &shader);

	friend Node &operator<<(Node &node, const Shader &shader);

private:
	static void IncrementDescriptorPool(std::map<VkDescriptorType, uint32_t> &descriptorPoolCounts, const VkDescriptorType &type);

	void LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, int32_t i);

	void LoadUniform(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, int32_t i);

	void LoadAttribute(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, int32_t i);

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

//REFLECT(VkVertexInputBindingDescription, {"binding", object.binding}, {"stride", object.stride}, {"inputRate", object.inputRate})
//REFLECT(VkVertexInputAttributeDescription, {"location", object.location}, {"binding", object.binding}, {"format", object.format}, {"offset", object.offset})

inline const Node &operator>>(const Node &node, VkVertexInputBindingDescription &bindingDescription)
{
	node["binding"].Get(bindingDescription.binding);
	node["stride"].Get(bindingDescription.stride);
	node["inputRate"].Get(bindingDescription.inputRate);
	return node;
}

inline Node &operator<<(Node &node, const VkVertexInputBindingDescription &bindingDescription)
{
	node["binding"].Set(bindingDescription.binding);
	node["stride"].Set(bindingDescription.stride);
	node["inputRate"].Set(bindingDescription.inputRate);
	return node;
}

inline const Node &operator>>(const Node &node, VkVertexInputAttributeDescription &attributeDescription)
{
	node["location"].Get(attributeDescription.location);
	node["binding"].Get(attributeDescription.binding);
	node["format"].Get(attributeDescription.format);
	node["offset"].Get(attributeDescription.offset);
	return node;
}

inline Node &operator<<(Node &node, const VkVertexInputAttributeDescription &attributeDescription)
{
	node["location"].Set(attributeDescription.location);
	node["binding"].Set(attributeDescription.binding);
	node["format"].Set(attributeDescription.format);
	node["offset"].Set(attributeDescription.offset);
	return node;
}
}

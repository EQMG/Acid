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
class ACID_EXPORT Shader
{
public:
	/// <summary>
	/// A define added to the start of a shader, first value is the define name and second is the value to be set.
	/// </summary>
	using Define = std::pair<std::string, std::string>;

	/// <summary>
	/// A class used to define sets of vertex inputs used in a shader.
	/// </summary>
	class VertexInput
	{
	public:
		explicit VertexInput(const uint32_t &binding = 0, std::vector<VkVertexInputBindingDescription> bindingDescriptions = {},
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {}) :
			m_binding(binding),
			m_bindingDescriptions(std::move(bindingDescriptions)),
			m_attributeDescriptions(std::move(attributeDescriptions))
		{
		}

		void Decode(const Metadata &metadata)
		{
			metadata.GetChild("Binding", m_binding);
			//	metadata.GetChild("Binding Descriptions", m_bindingDescriptions);
			//	metadata.GetChild("Attribute Descriptions", m_attributeDescriptions);
		}

		void Encode(Metadata &metadata) const
		{
			metadata.SetChild("Binding", m_binding);
			//	metadata.SetChild("Binding Descriptions", m_bindingDescriptions);
			//	metadata.SetChild("Attribute Descriptions", m_attributeDescriptions);
		}

		const uint32_t &GetBinding() const { return m_binding; }

		const std::vector<VkVertexInputBindingDescription> &GetBindingDescriptions() const { return m_bindingDescriptions; }

		const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return m_attributeDescriptions; }

		bool operator<(const VertexInput &other) const
		{
			return m_binding < other.m_binding;
		}

	private:
		uint32_t m_binding;
		std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
	};

	class Uniform
	{
	public:
		explicit Uniform(const int32_t &binding = -1, const int32_t &offset = -1, const int32_t &size = -1, const int32_t &glType = -1, const bool &readOnly = false, const bool &writeOnly = false,
			const VkShaderStageFlags &stageFlags = 0) :
			m_binding(binding),
			m_offset(offset),
			m_size(size),
			m_glType(glType),
			m_readOnly(readOnly),
			m_writeOnly(writeOnly),
			m_stageFlags(stageFlags)
		{
		}

		void Decode(const Metadata &metadata)
		{
			metadata.GetChild("Binding", m_binding);
			metadata.GetChild("Offset", m_offset);
			metadata.GetChild("Size", m_size);
			metadata.GetChild("GL Type", m_glType);
			metadata.GetChild("Read Only", m_readOnly);
			metadata.GetChild("Write Only", m_writeOnly);
			metadata.GetChild("Stage Flags", m_stageFlags);
		}

		void Encode(Metadata &metadata) const
		{
			metadata.SetChild("Binding", m_binding);
			metadata.SetChild("Offset", m_offset);
			metadata.SetChild("Size", m_size);
			metadata.SetChild("GL Type", m_glType);
			metadata.SetChild("Read Only", m_readOnly);
			metadata.SetChild("Write Only", m_writeOnly);
			metadata.SetChild("Stage Flags", m_stageFlags);
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
			std::stringstream result;
			result << "Uniform(binding " << m_binding << ", offset " << m_offset << ", size " << m_size << ", glType " << m_glType << ")";
			return result.str();
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
			Uniform, Storage, Push
		};

		explicit UniformBlock(const int32_t &binding = -1, const int32_t &size = -1, const VkShaderStageFlags &stageFlags = 0, const Type &type = Type::Uniform) :
			m_binding(binding),
			m_size(size),
			m_stageFlags(stageFlags),
			m_type(type)
		{
		}

		void Decode(const Metadata &metadata)
		{
			metadata.GetChild("Binding", m_binding);
			metadata.GetChild("Size", m_size);
			metadata.GetChild("Stage Flags", m_stageFlags);
			metadata.GetChild("Type", m_type);
			metadata.GetChild("Uniforms", m_uniforms);
		}

		void Encode(Metadata &metadata) const
		{
			metadata.SetChild("Binding", m_binding);
			metadata.SetChild("Size", m_size);
			metadata.SetChild("Stage Flags", m_stageFlags);
			metadata.SetChild("Type", m_type);
			metadata.SetChild("Uniforms", m_uniforms);
		}

		const int32_t &GetBinding() const { return m_binding; }

		const int32_t &GetSize() const { return m_size; }

		const VkShaderStageFlags &GetStageFlags() const { return m_stageFlags; }

		const Type &GetType() const { return m_type; }

		const std::map<std::string, Uniform> &GetUniforms() const { return m_uniforms; }

		std::optional<Uniform> GetUniform(const std::string &name) const
		{
			auto it = m_uniforms.find(name);

			if (it == m_uniforms.end())
			{
				return {};
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
			std::stringstream result;
			result << "UniformBlock(binding " << m_binding << ", size " << m_size << ", type " << static_cast<uint32_t>(m_type) << ")";
			return result.str();
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

		void Decode(const Metadata &metadata)
		{
			metadata.GetChild("Set", m_set);
			metadata.GetChild("Location", m_location);
			metadata.GetChild("Size", m_size);
			metadata.GetChild("GL Type", m_glType);
		}

		void Encode(Metadata &metadata) const
		{
			metadata.SetChild("Set", m_set);
			metadata.SetChild("Location", m_location);
			metadata.SetChild("Size", m_size);
			metadata.SetChild("GL Type", m_glType);
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
			std::stringstream result;
			result << "VertexAttribute(set " << m_set << "', location " << m_location << ", size " << m_size << ", glType " << m_glType << ")";
			return result.str();
		}

	private:
		friend class Shader;

		int32_t m_set;
		int32_t m_location;
		int32_t m_size;
		int32_t m_glType;
	};

	explicit Shader(std::string name);

	const std::string &GetName() const { return m_name; }

	bool ReportedNotFound(const std::string &name, const bool &reportIfFound) const;

	void ProcessShader();

	void Decode(const Metadata &metadata);

	void Encode(Metadata &metadata) const;

	static VkFormat GlTypeToVk(const int32_t &type);

	std::optional<uint32_t> GetDescriptorLocation(const std::string &name) const;

	std::optional<uint32_t> GetDescriptorSize(const std::string &name) const;

	std::optional<Uniform> GetUniform(const std::string &name) const;

	std::optional<UniformBlock> GetUniformBlock(const std::string &name) const;

	std::optional<Attribute> GetAttribute(const std::string &name) const;

	const uint32_t &GetLastDescriptorBinding() const { return m_lastDescriptorBinding; }

	const std::map<std::string, Uniform> &GetUniforms() const { return m_uniforms; };

	const std::map<std::string, UniformBlock> &GetUniformBlocks() const { return m_uniformBlocks; };

	const std::map<std::string, Attribute> &GetAttributes() const { return m_attributes; };

	const std::array<std::optional<uint32_t>, 3> &GetLocalSizes() const { return m_localSizes; }

	const std::vector<VkDescriptorSetLayoutBinding> &GetDescriptorSetLayouts() const { return m_descriptorSetLayouts; }

	const std::vector<VkDescriptorPoolSize> &GetDescriptorPools() const { return m_descriptorPools; }

	std::optional<VkDescriptorType> GetDescriptorType(const uint32_t &location) const;

	const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return m_attributeDescriptions; }

	static VkShaderStageFlagBits GetShaderStage(const std::string &filename);

	static std::string InsertDefineBlock(const std::string &shaderCode, const std::string &blockCode);

	static std::string ProcessIncludes(const std::string &shaderCode);

	VkShaderModule ProcessShader(const std::string &shaderCode, const VkShaderStageFlags &stageFlag);

	std::string ToString() const;

private:
	static void IncrementDescriptorPool(std::map<VkDescriptorType, uint32_t> &descriptorPoolCounts, const VkDescriptorType &type);

	void LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

	void LoadUniform(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

	void LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

	static int32_t ComputeSize(const glslang::TType *ttype);

	std::string m_name;
	std::map<std::string, Uniform> m_uniforms;
	std::map<std::string, UniformBlock> m_uniformBlocks;
	std::map<std::string, Attribute> m_attributes;

	std::array<std::optional<uint32_t>, 3> m_localSizes;

	std::map<std::string, uint32_t> m_descriptorLocations;
	std::map<std::string, uint32_t> m_descriptorSizes;

	std::vector<VkDescriptorSetLayoutBinding> m_descriptorSetLayouts;
	uint32_t m_lastDescriptorBinding;
	std::vector<VkDescriptorPoolSize> m_descriptorPools;
	std::map<uint32_t, VkDescriptorType> m_descriptorTypes;
	std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;

	mutable std::vector<std::string> m_notFoundNames;
};
}

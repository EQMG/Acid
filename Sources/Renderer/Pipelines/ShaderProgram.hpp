#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>
#include "Helpers/NonCopyable.hpp"

namespace glslang 
{
	class TProgram;
	class TType;
}

namespace acid
{
	class ACID_EXPORT Uniform
	{
	private:
		friend class ShaderProgram;
		std::string m_name;
		int32_t m_binding;
		int32_t m_offset;
		int32_t m_size;
		int32_t m_glType;
		bool m_readOnly;
		bool m_writeOnly;
		VkShaderStageFlags m_stageFlags;
	public:
		Uniform(const std::string &name, const int32_t &binding, const int32_t &offset, const int32_t &size, const int32_t &glType, const bool &readOnly, const bool &writeOnly, const VkShaderStageFlags &stageFlags) :
			m_name(name),
			m_binding(binding),
			m_offset(offset),
			m_size(size),
			m_glType(glType),
			m_readOnly(readOnly),
			m_writeOnly(writeOnly),
			m_stageFlags(stageFlags)
		{
		}

		const std::string &GetName() const { return m_name; }

		const int32_t &GetBinding() const { return m_binding; }

		const int32_t &GetOffset() const { return m_offset; }

		const int32_t &GetSize() const { return m_size; }

		const int32_t &GetGlType() const { return m_glType; }

		const bool &IsReadOnly() const { return m_readOnly; }

		const bool &IsWriteOnly() const { return m_writeOnly; }

		const VkShaderStageFlags &GetStageFlags() const { return m_stageFlags; }

		bool operator==(const Uniform &other) const
		{
			return m_name == other.m_name && m_binding == other.m_binding && m_offset == other.m_offset && m_glType == other.m_glType;
		}

		bool operator!=(const Uniform &other) const
		{
			return !(*this == other);
		}

		std::string ToString() const
		{
			std::stringstream result;
			result << "Uniform(name '" << m_name << "', binding " << m_binding << ", offset " << m_offset << ", size " << m_size << ", glType " << m_glType << ")";
			return result.str();
		}
	};

	enum UniformBlockType
	{
		BLOCK_UNIFORM = 0,
		BLOCK_STORAGE = 1,
		BLOCK_PUSH = 2
	};

	class ACID_EXPORT UniformBlock :
		public NonCopyable
	{
	private:
		friend class ShaderProgram;
		std::string m_name;
		int32_t m_binding;
		int32_t m_size;
		VkShaderStageFlags m_stageFlags;
		UniformBlockType m_type;
		std::vector<std::unique_ptr<Uniform>> m_uniforms;
	public:
		UniformBlock(const std::string &name, const int32_t &binding, const int32_t &size, const VkShaderStageFlags &stageFlags, const UniformBlockType &type) :
			m_name(name),
			m_binding(binding),
			m_size(size),
			m_stageFlags(stageFlags),
			m_type(type),
			m_uniforms(std::vector<std::unique_ptr<Uniform>>())
		{
		}

		void AddUniform(Uniform *uniform)
		{
			for (const auto &u : m_uniforms)
			{
				if (*u == *uniform)
				{
					return;
				}
			}

			m_uniforms.emplace_back(uniform);
		}

		const Uniform *GetUniform(const std::string &uniformName) const
		{
			for (const auto &uniform : m_uniforms)
			{
				if (uniform->GetName() == uniformName)
				{
					return uniform.get();
				}
			}

			return nullptr;
		}

		const std::string &GetName() const { return m_name; }

		const int32_t &GetBinding() const { return m_binding; }

		const int32_t &GetSize() const { return m_size; }

		const VkShaderStageFlags &GetStageFlags() const { return m_stageFlags; }

		const UniformBlockType &GetType() const { return m_type; }

		const std::vector<std::unique_ptr<Uniform>> &GetUniforms() { return m_uniforms; }

		std::string ToString() const
		{
			std::stringstream result;
			result << "UniformBlock(name '" << m_name << "', binding " << m_binding << ", size " << m_size << ", type " << m_type << ")";
			return result.str();
		}
	};

	class ACID_EXPORT VertexAttribute
	{
	private:
		std::string m_name;
		int32_t m_set;
		int32_t m_location;
		int32_t m_size;
		int32_t m_glType;
	public:
		VertexAttribute(const std::string &name, const int32_t &set, const int32_t &location, const int32_t &size, const int32_t &glType) :
			m_name(name),
			m_set(set),
			m_location(location),
			m_size(size),
			m_glType(glType)
		{
		}

		const std::string &GetName() const { return m_name; }

		const int32_t &GetSet() const { return m_set; }

		const int32_t &GetLocation() const { return m_location; }

		const int32_t &GetSize() const { return m_size; }

		const int32_t &GetGlType() const { return m_glType; }

		std::string ToString() const
		{
			std::stringstream result;
			result << "VertexAttribute(name '" << m_name << "', set " << m_set << "', location " << m_location << ", size " << m_size << ", glType " << m_glType << ")";
			return result.str();
		}
	};

	class ACID_EXPORT ShaderProgram :
		public NonCopyable
	{
	private:
		std::string m_name;
		std::vector<std::unique_ptr<Uniform>> m_uniforms;
		std::vector<std::unique_ptr<UniformBlock>> m_uniformBlocks;
		std::vector<std::unique_ptr<VertexAttribute>> m_vertexAttributes;

		std::vector<VkDescriptorSetLayoutBinding> m_descriptorSetLayouts;
		std::vector<VkDescriptorPoolSize> m_descriptorPools;
		std::vector<VkDescriptorType> m_descriptorTypes;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;

		mutable std::vector<std::string> m_notFoundNames;
	public:
		explicit ShaderProgram(const std::string &name);

		const std::string &GetName() const { return m_name; }

		bool ReportedNotFound(const std::string &name, const bool &reportIfFound) const;

		void ProcessShader();

		VkFormat GlTypeToVk(const int32_t &type);

		int32_t GetDescriptorLocation(const std::string &descriptor) const;

		std::optional<uint32_t> GetDescriptorSize(const std::string &descriptor) const;

		const Uniform *GetUniform(const std::string &uniformName) const;

		const UniformBlock *GetUniformBlock(const std::string &blockName) const;

		const VertexAttribute *GetVertexAttribute(const std::string &attributeName) const;

		uint32_t GetLastDescriptorBinding() const;

		const std::vector<std::unique_ptr<Uniform>> &GetUniforms() const { return m_uniforms; };

		const std::vector<std::unique_ptr<UniformBlock>> &GetUniformBlocks() const { return m_uniformBlocks; };

		const std::vector<std::unique_ptr<VertexAttribute>> &GetVertexAttributes() const { return m_vertexAttributes; };

		const std::vector<VkDescriptorSetLayoutBinding> &GetDescriptorSetLayouts() const { return m_descriptorSetLayouts; }

		const std::vector<VkDescriptorPoolSize> &GetDescriptorPools() const { return m_descriptorPools; }

		const VkDescriptorType &GetDescriptorType(const uint32_t &location) const { return m_descriptorTypes[location]; }

		const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return m_attributeDescriptions; }

		static VkShaderStageFlagBits GetShaderStage(const std::string &filename);

		static std::string InsertDefineBlock(const std::string &shaderCode, const std::string &blockCode);

		static std::string ProcessIncludes(const std::string &shaderCode);

		VkShaderModule ProcessShader(const std::string &shaderCode, const VkShaderStageFlags &stageFlag);

		std::string ToString() const;
	private:
		void IncrementDescriptorPool(std::map<VkDescriptorType, uint32_t> &descriptorPoolCounts, const VkDescriptorType &type);

		void LoadProgram(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag);

		void LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

		void LoadUniform(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

		void LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

		int32_t ComputeSize(const glslang::TType *ttype);
	};
}

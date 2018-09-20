#pragma once

#include <array>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>
#include "PipelineCreate.hpp"

namespace glslang 
{
	class TProgram;
}

namespace acid
{
	class ACID_EXPORT Uniform
	{
	private:
		std::string m_name;
		int32_t m_binding;
		int32_t m_offset;
		int32_t m_size;
		int32_t m_glType;
		VkShaderStageFlags m_stageFlags;
	public:
		Uniform(const std::string &name, const int32_t &binding, const int32_t &offset, const int32_t &size, const int32_t &glType, const VkShaderStageFlags &stageFlags) :
			m_name(name),
			m_binding(binding),
			m_offset(offset),
			m_size(size),
			m_glType(glType),
			m_stageFlags(stageFlags)
		{
		}

		std::string GetName() const { return m_name; }

		int32_t GetBinding() const { return m_binding; }

		int32_t GetOffset() const { return m_offset; }

		int32_t GetSize() const { return m_size; }

		int32_t GetGlType() const { return m_glType; }

		VkShaderStageFlags GetStageFlags() const { return m_stageFlags; }

		void SetStageFlags(const VkShaderStageFlags &stageFlags) { m_stageFlags = stageFlags; }

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

	class ACID_EXPORT UniformBlock
	{
	private:
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

		UniformBlock(const UniformBlock&) = delete; 

		UniformBlock& operator=(UniformBlock&) = delete;

		void AddUniform(Uniform *uniform)
		{
			for (auto &u : m_uniforms)
			{
				if (*u == *uniform)
				{
					return;
				}
			}

			m_uniforms.emplace_back(uniform);
		}

		Uniform *GetUniform(const std::string &uniformName)
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

		std::string GetName() const { return m_name; }

		int32_t GetBinding() const { return m_binding; }

		int32_t GetSize() const { return m_size; }

		VkShaderStageFlags GetStageFlags() const { return m_stageFlags; }

		void SetStageFlags(const VkShaderStageFlags &stageFlags) { m_stageFlags = stageFlags; }

		UniformBlockType GetType() const { return m_type; }

		std::vector<std::unique_ptr<Uniform>> &GetUniforms() { return m_uniforms; }

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

		std::string GetName() const { return m_name; }

		int32_t GetSet() const { return m_set; }

		int32_t GetLocation() const { return m_location; }

		int32_t GetSize() const { return m_size; }

		int32_t GetGlType() const { return m_glType; }

		std::string ToString() const
		{
			std::stringstream result;
			result << "VertexAttribute(name '" << m_name << "', set " << m_set << "', location " << m_location << ", size " << m_size << ", glType " << m_glType << ")";
			return result.str();
		}
	};

	class ACID_EXPORT ShaderProgram
	{
	private:
		std::string m_name;
		std::vector<std::unique_ptr<Uniform>> m_uniforms;
		std::vector<std::unique_ptr<UniformBlock>> m_uniformBlocks;
		std::vector<std::unique_ptr<VertexAttribute>> m_vertexAttributes;

		std::vector<DescriptorType> m_descriptors;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;

		std::vector<std::string> m_notFoundNames;
	public:
		explicit ShaderProgram(const std::string &name);

		ShaderProgram(const ShaderProgram&) = delete;

		ShaderProgram& operator=(ShaderProgram&) = delete;

		std::string GetName() const { return m_name; }

		bool ReportedNotFound(const std::string &name, const bool &reportIfFound);

		void ProcessShader();

		VkFormat GlTypeToVk(const int32_t &type);

		int32_t GetDescriptorLocation(const std::string &descriptor);

		Uniform *GetUniform(const std::string &uniformName);

		UniformBlock *GetUniformBlock(const std::string &blockName);

		VertexAttribute *GetVertexAttribute(const std::string &attributeName);

		const std::vector<std::unique_ptr<Uniform>> &GetUniforms() const { return m_uniforms; };

		const std::vector<std::unique_ptr<UniformBlock>> &GetUniformBlocks() const { return m_uniformBlocks; };

		const std::vector<std::unique_ptr<VertexAttribute>> &GetVertexAttributes() const { return m_vertexAttributes; };

		std::vector<DescriptorType> GetDescriptors() const { return m_descriptors; }

		uint32_t GetLastDescriptorBinding() const;

		std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() const { return m_attributeDescriptions; }

		static VkShaderStageFlagBits GetShaderStage(const std::string &filename);

		static std::string InsertDefineBlock(const std::string &shaderCode, const std::string &blockCode);

		static std::string ProcessIncludes(const std::string &shaderCode);

		VkShaderModule ProcessShader(const std::string &shaderCode, const VkShaderStageFlags &stageFlag);

		std::string ToString() const;

	private:
		void LoadProgram(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag);

		void LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

		void LoadUniform(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

		void LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);
	};
}

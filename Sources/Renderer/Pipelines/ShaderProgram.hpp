#pragma once

#include <array>
#include <string>
#include <sstream>
#include <vector>
#include "Display/Display.hpp"
#include "PipelineCreate.hpp"

namespace glslang
{
	class TProgram;
}

namespace fl
{
	class FL_HIDDEN Uniform
	{
	public:
		std::string m_name;
		int m_binding;
		int m_offset;
		int m_size;
		int m_glType;
		VkShaderStageFlagBits m_stageFlags;

		Uniform(const std::string &name, const int &binding, const int &offset, const int &size, const int &glType, const VkShaderStageFlagBits &stageFlags) :
			m_name(name),
			m_binding(binding),
			m_offset(offset),
			m_size(size),
			m_glType(glType),
			m_stageFlags(stageFlags)
		{
		}

		~Uniform()
		{
		}

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

	class FL_HIDDEN UniformBlock
	{
	public:
		std::string m_name;
		int m_binding;
		int m_size;
		VkShaderStageFlagBits m_stageFlags;
		std::vector<Uniform *> *m_uniforms;

		UniformBlock(const std::string &name, const int &binding, const int &size, const VkShaderStageFlagBits &stageFlags) :
			m_name(name),
			m_binding(binding),
			m_size(size),
			m_stageFlags(stageFlags),
			m_uniforms(new std::vector<Uniform *>())
		{
		}

		~UniformBlock()
		{
			delete m_uniforms;
		}

		void AddUniform(Uniform *uniform)
		{
			for (auto u : *m_uniforms)
			{
				if (*u == *uniform)
				{
					return;
				}
			}

			m_uniforms->push_back(uniform);
		}

		Uniform *GetUniform(const std::string &uniformName)
		{
			for (auto uniform : *m_uniforms)
			{
				if (uniform->m_name == uniformName)
				{
					return uniform;
				}
			}

			return nullptr;
		}

		std::string ToString() const
		{
			std::stringstream result;
			result << "UniformBlock(name '" << m_name << "', binding " << m_binding << ", size " << m_size << ")";
			return result.str();
		}
	};

	class FL_HIDDEN VertexAttribute
	{
	public:
		std::string m_name;
		int m_location;
		int m_size;
		int m_glType;

		VertexAttribute(const std::string &name, const int &location, const int &size, const int &glType) :
			m_name(name),
			m_location(location),
			m_size(size),
			m_glType(glType)
		{
		}

		~VertexAttribute()
		{
		}

		std::string ToString() const
		{
			std::stringstream result;
			result << "VertexAttribute(name '" << m_name << "', location " << m_location << ", size " << m_size << ", glType " << m_glType << ")";
			return result.str();
		}
	};

	class FL_HIDDEN ShaderProgram
	{
	private:
		std::vector<Uniform *> *m_uniforms;
		std::vector<UniformBlock *> *m_uniformBlocks;
		std::vector<VertexAttribute *> *m_vertexAttributes;

		std::vector<DescriptorType> *m_descriptors;
		std::vector<VkVertexInputAttributeDescription> *m_attributeDescriptions;
	public:
		ShaderProgram();

		~ShaderProgram();

		void LoadProgram(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag);

	private:
		void LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i);

		void LoadUniform(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i);

		void LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i);

	public:
		void ProcessShader();

		VkFormat GlTypeToVk(const int &type);

		int GetDescriptorLocation(const std::string &descriptor);

		Uniform *GetUniform(const std::string &uniformName);

		UniformBlock *GetUniformBlock(const std::string &blockName);

		VertexAttribute *GetVertexAttribute(const std::string &attributeName);

		std::vector<DescriptorType> *GetDescriptors() const { return m_descriptors; }

		std::vector<VkVertexInputAttributeDescription> *GetAttributeDescriptions() const { return m_attributeDescriptions; }

		static std::string InsertDefineBlock(const std::string &shaderCode, const std::string &blockCode);

		static VkShaderStageFlagBits GetShaderStage(const std::string &filename);

		std::string ToString() const;
	};
}

#pragma once

#include <array>
#include <string>
#include <vector>
#include <GlslangToSpv.h>
#include "../../Engine/Platform.hpp"
#include "PipelineCreate.hpp"
#include <sstream>

namespace Flounder
{
	class F_HIDDEN Uniform
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

	class F_HIDDEN UniformBlock
	{
	public:
		std::string m_name;
		int m_binding;
		int m_size;
		VkShaderStageFlagBits m_stageFlags;
		std::vector<Uniform*> *m_uniforms;

		UniformBlock(const std::string &name, const int &binding, const int &size, const VkShaderStageFlagBits &stageFlags) :
			m_name(name),
			m_binding(binding),
			m_size(size),
			m_stageFlags(stageFlags),
			m_uniforms(new std::vector<Uniform*>())
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

		std::string ToString() const
		{
			std::stringstream result;
			result << "UniformBlock(name '" << m_name << "', binding " << m_binding << ", size " << m_size << ")";
			return result.str();
		}
	};

	class F_HIDDEN VertexAttribute
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

	class F_HIDDEN ShaderProgram
	{
	public:
		std::vector<Uniform*> *m_uniforms;
		std::vector<UniformBlock*> *m_uniformBlocks;
		std::vector<VertexAttribute*> *m_vertexAttributes;

		std::vector<DescriptorType> *m_descriptors;

		ShaderProgram();

		~ShaderProgram();

		void LoadProgram(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag);

	private:
		void LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i);

		void LoadUniform(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i);

		void LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i);
	public:
		void ProcessShader();

		bool IsDescriptorDefined(const std::string &descriptor);

		UniformBlock *GetUniformBlock(const std::string &blockName);

		Uniform *GetBlockUniform(const std::string &blockName, const std::string &uniformName);

		static std::string InsertDefineBlock(const std::string &shaderCode, const std::string &blockCode);

		static VkShaderStageFlagBits GetShaderStage(const std::string &filename);

		static EShLanguage GetEshLanguage(const VkShaderStageFlagBits &stageFlag);

		static TBuiltInResource GetResources();
	};
}

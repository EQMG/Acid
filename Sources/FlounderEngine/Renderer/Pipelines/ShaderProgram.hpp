#pragma once

#include <array>
#include <string>
#include <vector>
#include <GlslangToSpv.h>
#include "../../Engine/Platform.hpp"
#include "PipelineCreate.hpp"

namespace Flounder
{
	enum BasicTypes
	{
		TypeBool = 35670,
		TypeInt = 5124,
		TypeFloat = 5126,
		TypeDouble = 5130,
		TypeVec2 = 35664,
		TypeVec3 = 35665,
		TypeVec4 = 35666,
		TypeMat2 = 35674,
		TypeMat3 = 35675,
		TypeMat4 = 35676,
		TypeSampler2D = 35678,
		TypeSampler3D = 35680,
		TypeWrite2D = 36941,
		TypeWrite3D = 36942,
	};

	class F_HIDDEN Uniform
	{
	public:
		std::string m_name;
		int m_binding;
		int m_offset;
		BasicTypes m_type;
		VkShaderStageFlagBits m_stageFlags;

		Uniform(const std::string &name, const int &binding, const int &offset, const int &type, const VkShaderStageFlagBits &stageFlags) :
			m_name(name),
			m_binding(binding),
			m_offset(offset),
			m_type((BasicTypes)type),
			m_stageFlags(stageFlags)
		{
		}

		~Uniform()
		{
		}

		bool operator==(const Uniform &other) const
		{
			return m_name == other.m_name && m_binding == other.m_binding && m_offset == other.m_offset && m_type == other.m_type;
		}

		bool operator!=(const Uniform &other) const
		{
			return !(*this == other);
		}

		std::string ToString() const
		{
			std::stringstream result;
			result << "Uniform(name '" << m_name << "', binding " << m_binding << ", offset " << m_offset << ", type " << m_type << ")";
			return result.str();
		}
	};

	class F_HIDDEN UniformBlock
	{
	public:
		std::string m_name;
		int m_index;
		int m_size;
		VkShaderStageFlagBits m_stageFlags;
		std::vector<Uniform*> *m_uniforms;

		UniformBlock(const std::string &name, const int &index, const int &size, const VkShaderStageFlagBits &stageFlags) :
			m_name(name),
			m_index(index),
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
			result << "UniformBlock(name '" << m_name << "', index " << m_index << ", size " << m_size << ")";
			return result.str();
		}
	};

	class F_HIDDEN VertexAttribute
	{
	public:
		std::string m_name;
		int m_index;
		BasicTypes m_type;

		VertexAttribute(const std::string &name, const int &index, const int &type) :
			m_name(name),
			m_index(index),
			m_type((BasicTypes)type)
		{
		}

		~VertexAttribute()
		{
		}

		std::string ToString() const
		{
			std::stringstream result;
			result << "VertexAttribute(name '" << m_name << "', index " << m_index << ", type " << m_type << ")";
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
		void LoadUniform(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i);

		void LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i);

		void LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlagBits &stageFlag, const int &i);
	public:
		void ProcessShader();

		static std::string InsertDefineBlock(const std::string &shaderCode, const std::string &blockCode);

		static VkShaderStageFlagBits GetShaderStage(const std::string &filename);

		static EShLanguage GetEshLanguage(const VkShaderStageFlagBits &stageFlag);

		static TBuiltInResource GetResources();
	};
}

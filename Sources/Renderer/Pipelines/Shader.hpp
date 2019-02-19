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
	class ACID_EXPORT Shader :
		public NonCopyable
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
			VertexInput(const uint32_t &binding, const std::vector<VkVertexInputBindingDescription> &bindingDescriptions, const std::vector<VkVertexInputAttributeDescription> &attributeDescriptions) :
				m_binding(binding),
				m_bindingDescriptions(bindingDescriptions),
				m_attributeDescriptions(attributeDescriptions)
			{
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
			Uniform(const int32_t &binding, const int32_t &offset, const int32_t &size, const int32_t &glType, const bool &readOnly, const bool &writeOnly, const VkShaderStageFlags &stageFlags) :
				m_binding(binding),
				m_offset(offset),
				m_size(size),
				m_glType(glType),
				m_readOnly(readOnly),
				m_writeOnly(writeOnly),
				m_stageFlags(stageFlags)
			{
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
				return m_binding == other.m_binding && m_offset == other.m_offset && m_glType == other.m_glType;
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

		class UniformBlock :
			public NonCopyable
		{
		public:
			enum class Type
			{
				Uniform,
				Storage,
				Push
			};

			UniformBlock(const int32_t &binding, const int32_t &size, const VkShaderStageFlags &stageFlags, const Type &type) :
				m_binding(binding),
				m_size(size),
				m_stageFlags(stageFlags),
				m_type(type),
				m_uniforms(std::map<std::string, std::unique_ptr<Uniform>>())
			{
			}

			const int32_t &GetBinding() const { return m_binding; }

			const int32_t &GetSize() const { return m_size; }

			const VkShaderStageFlags &GetStageFlags() const { return m_stageFlags; }

			const Type &GetType() const { return m_type; }

			const std::map<std::string, std::unique_ptr<Uniform>> &GetUniforms() { return m_uniforms; }

			const Uniform *GetUniform(const std::string &name) const
			{
				auto it = m_uniforms.find(name);

				if (it == m_uniforms.end())
				{
					return nullptr;
				}

				return it->second.get();
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
			std::map<std::string, std::unique_ptr<Uniform>> m_uniforms;
		};

		class Attribute
		{
		public:
			Attribute(const int32_t &set, const int32_t &location, const int32_t &size, const int32_t &glType) :
				m_set(set),
				m_location(location),
				m_size(size),
				m_glType(glType)
			{
			}

			const int32_t &GetSet() const { return m_set; }

			const int32_t &GetLocation() const { return m_location; }

			const int32_t &GetSize() const { return m_size; }

			const int32_t &GetGlType() const { return m_glType; }

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

		explicit Shader(const std::string &name);

		const std::string &GetName() const { return m_name; }

		bool ReportedNotFound(const std::string &name, const bool &reportIfFound) const;

		void ProcessShader();

		VkFormat GlTypeToVk(const int32_t &type);

		int32_t GetDescriptorLocation(const std::string &descriptor) const;

		std::optional<uint32_t> GetDescriptorSize(const std::string &descriptor) const;

		const Uniform *GetUniform(const std::string &name) const;

		const UniformBlock *GetUniformBlock(const std::string &name) const;

		const Attribute *GetAttribute(const std::string &name) const;

		uint32_t GetLastDescriptorBinding() const;

		const std::map<std::string, std::unique_ptr<Uniform>> &GetUniforms() const { return m_uniforms; };

		const std::map<std::string, std::unique_ptr<UniformBlock>> &GetUniformBlocks() const { return m_uniformBlocks; };

		const std::map<std::string, std::unique_ptr<Attribute>> &GetAttributes() const { return m_attributes; };

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
		void IncrementDescriptorPool(std::map<VkDescriptorType, uint32_t> &descriptorPoolCounts, const VkDescriptorType &type);

		void LoadProgram(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag);

		void LoadUniformBlock(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

		void LoadUniform(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

		void LoadVertexAttribute(const glslang::TProgram &program, const VkShaderStageFlags &stageFlag, const int32_t &i);

		int32_t ComputeSize(const glslang::TType *ttype);

		std::string m_name;
		std::map<std::string, std::unique_ptr<Uniform>> m_uniforms;
		std::map<std::string, std::unique_ptr<UniformBlock>> m_uniformBlocks;
		std::map<std::string, std::unique_ptr<Attribute>> m_attributes;

		std::vector<VkDescriptorSetLayoutBinding> m_descriptorSetLayouts;
		std::vector<VkDescriptorPoolSize> m_descriptorPools;
		std::map<uint32_t, VkDescriptorType> m_descriptorTypes;
		std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;

		mutable std::vector<std::string> m_notFoundNames;
	};
}

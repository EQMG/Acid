#pragma once

#include <array>
#include <volk.h>

#include "Files/Node.hpp"
#include "Graphics/Export.hpp"

namespace glslang {
class TProgram;
class TType;
}

namespace acid {
/**
 * @brief Class that loads and processes a shader, and provides a reflection.
 */
class ACID_GRAPHICS_EXPORT Shader {
public:
	/**
	 * A define added to the start of a shader, first value is the define name and second is the value to be set.
	 */
	using Define = std::pair<std::string, std::string>;

	/**
	 * @brief Class used to define sets of vertex inputs used in a shader.
	 */
	class VertexInput {
	public:
		VertexInput(std::vector<VkVertexInputBindingDescription> bindingDescriptions = {}, std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {}) :
			bindingDescriptions(std::move(bindingDescriptions)),
			attributeDescriptions(std::move(attributeDescriptions)) {
		}

		const std::vector<VkVertexInputBindingDescription> &GetBindingDescriptions() const { return bindingDescriptions; }
		const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return attributeDescriptions; }

		bool operator<(const VertexInput &rhs) const {
			return bindingDescriptions.front().binding < rhs.bindingDescriptions.front().binding;
		}

		friend const Node &operator>>(const Node &node, VertexInput &vertexInput) {
			node["bindingDescriptions"].Get(vertexInput.bindingDescriptions);
			node["attributeDescriptions"].Get(vertexInput.attributeDescriptions);
			return node;
		}

		friend Node &operator<<(Node &node, const VertexInput &vertexInput) {
			node["bindingDescriptions"].Set(vertexInput.bindingDescriptions);
			node["attributeDescriptions"].Set(vertexInput.attributeDescriptions);
			return node;
		}

	private:
		uint32_t binding = 0;
		std::vector<VkVertexInputBindingDescription> bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	};

	class Uniform {
		friend class Shader;
	public:
		explicit Uniform(int32_t binding = -1, int32_t offset = -1, int32_t size = -1, int32_t glType = -1, bool readOnly = false,
			bool writeOnly = false, VkShaderStageFlags stageFlags = 0) :
			binding(binding),
			offset(offset),
			size(size),
			glType(glType),
			readOnly(readOnly),
			writeOnly(writeOnly),
			stageFlags(stageFlags) {
		}

		int32_t GetBinding() const { return binding; }
		int32_t GetOffset() const { return offset; }
		int32_t GetSize() const { return size; }
		int32_t GetGlType() const { return glType; }
		bool IsReadOnly() const { return readOnly; }
		bool IsWriteOnly() const { return writeOnly; }
		VkShaderStageFlags GetStageFlags() const { return stageFlags; }

		bool operator==(const Uniform &rhs) const {
			return binding == rhs.binding && offset == rhs.offset && size == rhs.size && glType == rhs.glType && readOnly == rhs.readOnly && 
				writeOnly == rhs.writeOnly && stageFlags == rhs.stageFlags;
		}

		bool operator!=(const Uniform &rhs) const {
			return !operator==(rhs);
		}

		friend const Node &operator>>(const Node &node, Uniform &uniform) {
			node["binding"].Get(uniform.binding);
			node["offset"].Get(uniform.offset);
			node["size"].Get(uniform.size);
			node["glType"].Get(uniform.glType);
			node["readOnly"].Get(uniform.readOnly);
			node["writeOnly"].Get(uniform.writeOnly);
			node["stageFlags"].Get(uniform.stageFlags);
			return node;
		}

		friend Node &operator<<(Node &node, const Uniform &uniform) {
			node["binding"].Set(uniform.binding);
			node["offset"].Set(uniform.offset);
			node["size"].Set(uniform.size);
			node["glType"].Set(uniform.glType);
			node["readOnly"].Set(uniform.readOnly);
			node["writeOnly"].Set(uniform.writeOnly);
			node["stageFlags"].Set(uniform.stageFlags);
			return node;
		}

	private:
		int32_t binding;
		int32_t offset;
		int32_t size;
		int32_t glType;
		bool readOnly;
		bool writeOnly;
		VkShaderStageFlags stageFlags;
	};

	class UniformBlock {
		friend class Shader;
	public:
		enum class Type { None, Uniform, Storage, Push };

		explicit UniformBlock(int32_t binding = -1, int32_t size = -1, VkShaderStageFlags stageFlags = 0, Type type = Type::Uniform) :
			binding(binding),
			size(size),
			stageFlags(stageFlags),
			type(type) {
		}

		int32_t GetBinding() const { return binding; }
		int32_t GetSize() const { return size; }
		VkShaderStageFlags GetStageFlags() const { return stageFlags; }
		Type GetType() const { return type; }
		const std::map<std::string, Uniform> &GetUniforms() const { return uniforms; }

		std::optional<Uniform> GetUniform(const std::string &name) const {
			auto it = uniforms.find(name);

			if (it == uniforms.end()) {
				return std::nullopt;
			}

			return it->second;
		}

		bool operator==(const UniformBlock &rhs) const {
			return binding == rhs.binding && size == rhs.size && stageFlags == rhs.stageFlags && type == rhs.type && uniforms == rhs.uniforms;
		}

		bool operator!=(const UniformBlock &rhs) const {
			return !operator==(rhs);
		}

		friend const Node &operator>>(const Node &node, UniformBlock &uniformBlock) {
			node["binding"].Get(uniformBlock.binding);
			node["size"].Get(uniformBlock.size);
			node["stageFlags"].Get(uniformBlock.stageFlags);
			node["type"].Get(uniformBlock.type);
			node["uniforms"].Get(uniformBlock.uniforms);
			return node;
		}

		friend Node &operator<<(Node &node, const UniformBlock &uniformBlock) {
			node["binding"].Set(uniformBlock.binding);
			node["size"].Set(uniformBlock.size);
			node["stageFlags"].Set(uniformBlock.stageFlags);
			node["type"].Set(uniformBlock.type);
			node["uniforms"].Set(uniformBlock.uniforms);
			return node;
		}

	private:
		int32_t binding;
		int32_t size;
		VkShaderStageFlags stageFlags;
		Type type;
		std::map<std::string, Uniform> uniforms;
	};

	class Attribute {
		friend class Shader;
	public:
		explicit Attribute(int32_t set = -1, int32_t location = -1, int32_t size = -1, int32_t glType = -1) :
			set(set),
			location(location),
			size(size),
			glType(glType) {
		}

		int32_t GetSet() const { return set; }
		int32_t GetLocation() const { return location; }
		int32_t GetSize() const { return size; }
		int32_t GetGlType() const { return glType; }

		bool operator==(const Attribute &rhs) const {
			return set == rhs.set && location == rhs.location && size == rhs.size && glType == rhs.glType;
		}

		bool operator!=(const Attribute &rhs) const {
			return !operator==(rhs);
		}

		friend const Node &operator>>(const Node &node, Attribute &attribute) {
			node["set"].Get(attribute.set);
			node["location"].Get(attribute.location);
			node["size"].Get(attribute.size);
			node["glType"].Get(attribute.glType);
			return node;
		}

		friend Node &operator<<(Node &node, const Attribute &attribute) {
			node["set"].Set(attribute.set);
			node["location"].Set(attribute.location);
			node["size"].Set(attribute.size);
			node["glType"].Set(attribute.glType);
			return node;
		}

	private:
		int32_t set;
		int32_t location;
		int32_t size;
		int32_t glType;
	};

	class Constant {
		friend class Shader;
	public:
		explicit Constant(int32_t binding = -1, int32_t size = -1, VkShaderStageFlags stageFlags = 0, int32_t glType = -1) :
			binding(binding),
			size(size),
			stageFlags(stageFlags),
			glType(glType) {
		}

		int32_t GetBinding() const { return binding; }
		int32_t GetSize() const { return size; }
		VkShaderStageFlags GetStageFlags() const { return stageFlags; }
		int32_t GetGlType() const { return glType; }

		bool operator==(const Constant &rhs) const {
			return binding == rhs.binding && size == rhs.size && stageFlags == rhs.stageFlags && glType == rhs.glType;
		}

		bool operator!=(const Constant &rhs) const {
			return !operator==(rhs);
		}

		friend const Node &operator>>(const Node &node, Constant &constant) {
			node["binding"].Get(constant.binding);
			node["size"].Get(constant.size);
			node["stageFlags"].Get(constant.stageFlags);
			node["glType"].Get(constant.glType);
			return node;
		}

		friend Node &operator<<(Node &node, const Constant &constant) {
			node["binding"].Set(constant.binding);
			node["size"].Set(constant.size);
			node["stageFlags"].Set(constant.stageFlags);
			node["glType"].Set(constant.glType);
			return node;
		}

	private:
		int32_t binding;
		int32_t size;
		VkShaderStageFlags stageFlags;
		int32_t glType;
	};

	Shader();

	bool ReportedNotFound(const std::string &name, bool reportIfFound) const;
	static VkFormat GlTypeToVk(int32_t type);
	std::optional<uint32_t> GetDescriptorLocation(const std::string &name) const;
	std::optional<uint32_t> GetDescriptorSize(const std::string &name) const;
	std::optional<Uniform> GetUniform(const std::string &name) const;
	std::optional<UniformBlock> GetUniformBlock(const std::string &name) const;
	std::optional<Attribute> GetAttribute(const std::string &name) const;
	std::vector<VkPushConstantRange> GetPushConstantRanges() const;

	std::optional<VkDescriptorType> GetDescriptorType(uint32_t location) const;
	static VkShaderStageFlagBits GetShaderStage(const std::filesystem::path &filename);
	VkShaderModule CreateShaderModule(const std::filesystem::path &moduleName, const std::string &moduleCode, const std::string &preamble, VkShaderStageFlags moduleFlag);
	void CreateReflection();

	const std::filesystem::path &GetName() const { return stages.back(); }
	uint32_t GetLastDescriptorBinding() const { return lastDescriptorBinding; }
	const std::map<std::string, Uniform> &GetUniforms() const { return uniforms; };
	const std::map<std::string, UniformBlock> &GetUniformBlocks() const { return uniformBlocks; };
	const std::map<std::string, Attribute> &GetAttributes() const { return attributes; };
	const std::map<std::string, Constant> &GetConstants() const { return constants; };
	const std::array<std::optional<uint32_t>, 3> &GetLocalSizes() const { return localSizes; }
	const std::vector<VkDescriptorSetLayoutBinding> &GetDescriptorSetLayouts() const { return descriptorSetLayouts; }
	const std::vector<VkDescriptorPoolSize> &GetDescriptorPools() const { return descriptorPools; }
	const std::vector<VkVertexInputAttributeDescription> &GetAttributeDescriptions() const { return attributeDescriptions; }

	friend const Node &operator>>(const Node &node, Shader &shader);
	friend Node &operator<<(Node &node, const Shader &shader);

private:
	static void IncrementDescriptorPool(std::map<VkDescriptorType, uint32_t> &descriptorPoolCounts, VkDescriptorType type);
	void LoadUniformBlock(const glslang::TProgram &program, VkShaderStageFlags stageFlag, int32_t i);
	void LoadUniform(const glslang::TProgram &program, VkShaderStageFlags stageFlag, int32_t i);
	void LoadAttribute(const glslang::TProgram &program, VkShaderStageFlags stageFlag, int32_t i);
	static int32_t ComputeSize(const glslang::TType *ttype);

	std::vector<std::filesystem::path> stages;
	std::map<std::string, Uniform> uniforms;
	std::map<std::string, UniformBlock> uniformBlocks;
	std::map<std::string, Attribute> attributes;
	std::map<std::string, Constant> constants;

	std::array<std::optional<uint32_t>, 3> localSizes;

	std::map<std::string, uint32_t> descriptorLocations;
	std::map<std::string, uint32_t> descriptorSizes;

	std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayouts;
	uint32_t lastDescriptorBinding = 0;
	std::vector<VkDescriptorPoolSize> descriptorPools;
	std::map<uint32_t, VkDescriptorType> descriptorTypes;
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

	mutable std::vector<std::string> notFoundNames;
};

inline const Node &operator>>(const Node &node, VkVertexInputBindingDescription &bindingDescription) {
	node["binding"].Get(bindingDescription.binding);
	node["stride"].Get(bindingDescription.stride);
	node["inputRate"].Get(bindingDescription.inputRate);
	return node;
}

inline Node &operator<<(Node &node, const VkVertexInputBindingDescription &bindingDescription) {
	node["binding"].Set(bindingDescription.binding);
	node["stride"].Set(bindingDescription.stride);
	node["inputRate"].Set(bindingDescription.inputRate);
	return node;
}

inline const Node &operator>>(const Node &node, VkVertexInputAttributeDescription &attributeDescription) {
	node["location"].Get(attributeDescription.location);
	node["binding"].Get(attributeDescription.binding);
	node["format"].Get(attributeDescription.format);
	node["offset"].Get(attributeDescription.offset);
	return node;
}

inline Node &operator<<(Node &node, const VkVertexInputAttributeDescription &attributeDescription) {
	node["location"].Set(attributeDescription.location);
	node["binding"].Set(attributeDescription.binding);
	node["format"].Set(attributeDescription.format);
	node["offset"].Set(attributeDescription.offset);
	return node;
}
}

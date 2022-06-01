#pragma once

#include <cstring>
#include <functional>
#include <unordered_map>

#include "Maths/Vector3.hpp"
#include "Resources/Resource.hpp"
#include "Graphics/Buffers/Buffer.hpp"

namespace acid {
template<typename Base>
class ModelFactory {
public:
	using TCreateReturn = std::shared_ptr<Base>;
	
	using TCreateMethodNode = std::function<TCreateReturn(const Node &)>;
	using TRegistryMapNode = std::unordered_map<std::string, TCreateMethodNode>;

	using TCreateMethodFilename = std::function<TCreateReturn(const std::filesystem::path &)>;
	using TRegistryMapFilename = std::unordered_map<std::string, TCreateMethodFilename>;

	virtual ~ModelFactory() = default;

	/**
	 * Creates a new model, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The model with the requested values.
	 */
	static TCreateReturn Create(const Node &node) {
		auto typeName = node["type"].Get<std::string>();
		auto it = RegistryNode().find(typeName);
		return it == RegistryNode().end() ? nullptr : it->second(node);
	}

	/**
	 * Creates a new model, or finds one with the same values.
	 * @param filename The file to load the model from.
	 * @return The model loaded from the filename.
	 */
	static TCreateReturn Create(const std::filesystem::path &filename) {
		auto fileExt = filename.extension().string();
		auto it = RegistryFilename().find(fileExt);
		return it == RegistryFilename().end() ? nullptr : it->second(filename);
	}

	static TRegistryMapNode &RegistryNode() {
		static TRegistryMapNode impl;
		return impl;
	}

	static TRegistryMapFilename &RegistryFilename() {
		static TRegistryMapFilename impl;
		return impl;
	}

	/**
	 * A class used to help register subclasses of Model to the factory.
	 * Your subclass should have a static Create function taking a Node, or path.
	 * @tparam T The type that will extend Model.
	 */
	template<typename T>
	class Registrar : public Base {
	protected:
		template<int Dummy = 0>
		static bool Register(const std::string &typeName) {
			name = typeName;
			ModelFactory::RegistryNode()[typeName] = [](const Node &node) -> TCreateReturn {
				return T::Create(node);
			};
			return true;
		}

		template<int Dummy = 0>
		static bool Register(const std::string &typeName, const std::string &extension) {
			Register(typeName);
			ModelFactory::RegistryFilename()[extension] = [](const std::filesystem::path &filename) -> TCreateReturn {
				return T::Create(filename);
			};
			return true;
		}

		const Node &Load(const Node &node) override {
			return node >> *dynamic_cast<T *>(this);
		}

		Node &Write(Node &node) const override {
			node["type"].Set(name);
			return node << *dynamic_cast<const T *>(this);
		}

		inline static std::string name;
	};

	friend const Node &operator>>(const Node &node, Base &base) {
		return base.Load(node);
	}

	friend Node &operator<<(Node &node, const Base &base) {
		return base.Write(node);
	}
	
protected:
	virtual const Node &Load(const Node &node) { return node; }
	virtual Node &Write(Node &node) const { return node; }
};

/**
 * @brief Resource that represents a model vertex and index buffer.
 */
class ACID_GRAPHICS_EXPORT Model : public ModelFactory<Model>, public Resource {
public:
	/**
	 * Creates a new empty model.
	 */
	Model() = default;

	/**
	 * Creates a new model.
	 * @tparam T The vertex type.
	 * @param vertices The model vertices.
	 * @param indices The model indices.
	 */
	template<typename T>
	explicit Model(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {});

	bool CmdRender(const CommandBuffer &commandBuffer, uint32_t instances = 1) const;

	std::type_index GetTypeIndex() const override { return typeid(Model); }

	template<typename T>
	std::vector<T> GetVertices(std::size_t offset = 0) const;
	template<typename T>
	void SetVertices(const std::vector<T> &vertices);

	std::vector<uint32_t> GetIndices(std::size_t offset = 0) const;
	void SetIndices(const std::vector<uint32_t> &indices);

	std::vector<float> GetPointCloud() const;

	const Vector3f &GetMinExtents() const { return minExtents; }
	const Vector3f &GetMaxExtents() const { return maxExtents; }
	float GetWidth() const { return maxExtents.x - minExtents.x; }
	float GetHeight() const { return maxExtents.y - minExtents.y; }
	float GetDepth() const { return maxExtents.z - minExtents.z; }
	float GetRadius() const { return radius; }
	const Buffer *GetVertexBuffer() const { return vertexBuffer.get(); }
	const Buffer *GetIndexBuffer() const { return indexBuffer.get(); }
	uint32_t GetVertexCount() const { return vertexCount; }
	uint32_t GetIndexCount() const { return indexCount; }
	static VkIndexType GetIndexType() { return VK_INDEX_TYPE_UINT32; }

protected:
	template<typename T>
	void Initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {});

private:
	std::unique_ptr<Buffer> vertexBuffer;
	std::unique_ptr<Buffer> indexBuffer;
	uint32_t vertexCount = 0;
	uint32_t indexCount = 0;

	Vector3f minExtents;
	Vector3f maxExtents;
	float radius = 0.0f;
};

template<typename T>
Model::Model(const std::vector<T> &vertices, const std::vector<uint32_t> &indices) :
	Model() {
	Initialize(vertices, indices);
}

template<typename T>
std::vector<T> Model::GetVertices(std::size_t offset) const {
	Buffer vertexStaging(vertexBuffer->GetSize(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	CommandBuffer commandBuffer;

	VkBufferCopy copyRegion = {};
	copyRegion.size = vertexStaging.GetSize();
	vkCmdCopyBuffer(commandBuffer, vertexBuffer->GetBuffer(), vertexStaging.GetBuffer(), 1, &copyRegion);

	commandBuffer.SubmitIdle();

	void *verticesMemory;
	vertexStaging.MapMemory(&verticesMemory);
	std::vector<T> vertices(vertexCount);

	auto sizeOfSrcT = vertexStaging.GetSize() / vertexCount;

	for (uint32_t i = 0; i < vertexCount; i++) {
		std::memcpy(&vertices[i], static_cast<char *>(verticesMemory) + (i * sizeOfSrcT) + offset, sizeof(T));
	}

	vertexStaging.UnmapMemory();
	return vertices;
}

template<typename T>
void Model::SetVertices(const std::vector<T> &vertices) {
	vertexBuffer = nullptr;
	vertexCount = static_cast<uint32_t>(vertices.size());

	if (vertices.empty())
		return;

	Buffer vertexStaging(sizeof(T) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		vertices.data());
	vertexBuffer = std::make_unique<Buffer>(vertexStaging.GetSize(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	CommandBuffer commandBuffer;

	VkBufferCopy copyRegion = {};
	copyRegion.size = vertexStaging.GetSize();
	vkCmdCopyBuffer(commandBuffer, vertexStaging.GetBuffer(), vertexBuffer->GetBuffer(), 1, &copyRegion);

	commandBuffer.SubmitIdle();
}

template<typename T>
void Model::Initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices) {
	SetVertices(vertices);
	SetIndices(indices);

	minExtents = Vector3f::Infinity;
	maxExtents = -Vector3f::Infinity;

	for (const auto &vertex : vertices) {
		Vector3f position(vertex.position);
		minExtents = minExtents.Min(position);
		maxExtents = maxExtents.Max(position);
	}

	radius = std::max(minExtents.Length(), maxExtents.Length());
}
}

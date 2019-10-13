#pragma once

#include "Maths/Vector3.hpp"
#include "Graphics/Buffers/Buffer.hpp"
#include "Resources/Resource.hpp"

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
			return node << *dynamic_cast<const T *>(this);
		}
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
class ACID_EXPORT Model : public ModelFactory<Model>, public Resource {
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
	explicit Model(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {}) :
		Model() {
		Initialize(vertices, indices);
	}

	bool CmdRender(const CommandBuffer &commandBuffer, uint32_t instances = 1) const;

	template<typename T>
	std::vector<T> GetVertices(std::size_t offset = 0) const;
	template<typename T>
	void SetVertices(const std::vector<T> &vertices);

	std::vector<uint32_t> GetIndices(std::size_t offset = 0) const;
	void SetIndices(const std::vector<uint32_t> &indices);

	std::vector<float> GetPointCloud() const;

	const Vector3f &GetMinExtents() const { return m_minExtents; }
	const Vector3f &GetMaxExtents() const { return m_maxExtents; }
	float GetWidth() const { return m_maxExtents.m_x - m_minExtents.m_x; }
	float GetHeight() const { return m_maxExtents.m_y - m_minExtents.m_y; }
	float GetDepth() const { return m_maxExtents.m_z - m_minExtents.m_z; }
	float GetRadius() const { return m_radius; }
	const Buffer *GetVertexBuffer() const { return m_vertexBuffer.get(); }
	const Buffer *GetIndexBuffer() const { return m_indexBuffer.get(); }
	uint32_t GetVertexCount() const { return m_vertexCount; }
	uint32_t GetIndexCount() const { return m_indexCount; }
	static VkIndexType GetIndexType() { return VK_INDEX_TYPE_UINT32; }

protected:
	template<typename T>
	void Initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {});

private:
	std::unique_ptr<Buffer> m_vertexBuffer;
	std::unique_ptr<Buffer> m_indexBuffer;
	uint32_t m_vertexCount = 0;
	uint32_t m_indexCount = 0;

	Vector3f m_minExtents;
	Vector3f m_maxExtents;
	float m_radius = 0.0f;
};
}

#include "Model.inl"

#pragma once

#include "../Objects/GameObject.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Space/ISpatialStructure.hpp"

namespace Flounder
{
	class F_EXPORT Entity :
		public GameObject,
		public ISpatialObject
	{
	private:
		UniformBuffer *m_uniformObject;

		ISpatialStructure<Entity *> *m_structure;

		std::string m_prefabName;
		bool m_removed;
	public:
		Entity(const std::string &prefabName, const Transform &transform, ISpatialStructure<Entity *> *structure = nullptr);

		Entity(const Transform &transform, ISpatialStructure<Entity *> *structure = nullptr);

		~Entity();

		void Update() override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene);

		ISpatialStructure<Entity *> *GetStructure() const { return m_structure; }

		void MoveStructure(ISpatialStructure<Entity *> *structure);

		std::string GetPrefabName() const { return m_prefabName; }

		void SetPrefabName(const std::string &prefabName) { m_prefabName = prefabName; }

		bool GetRemoved() const { return m_removed; }

		void Remove();

		ICollider *GetCollider() override;
	};
}

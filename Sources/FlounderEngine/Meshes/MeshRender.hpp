#pragma once

#include <vector>
#include "../Objects/Component.hpp"
#include "../Objects/GameObject.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	/// <summary>
	/// Class used to render a mesh.
	/// </summary>
	class F_EXPORT MeshRender :
		public Component
	{
	private:
		UniformBuffer *m_uniformObject;
		DescriptorSet *m_descriptorSet;
	public:
		MeshRender();

		~MeshRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, UniformBuffer *uniformScene);

		std::string GetName() const override { return "MeshRender"; };

		UniformBuffer *GetUniformObject() const { return m_uniformObject; }
	};
}

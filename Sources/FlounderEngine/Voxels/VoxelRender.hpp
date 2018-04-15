#pragma once

#include <vector>
#include "../Objects/Component.hpp"
#include "../Objects/GameObject.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	/// <summary>
	/// Class used to render a GameObject as a entity.
	/// </summary>
	class F_EXPORT VoxelRender :
		public Component
	{
	private:
		UniformBuffer *m_uniformObject;
		DescriptorSet *m_descriptorSet;
	public:
		VoxelRender();

		~VoxelRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformBuffer *uniformScene);

		std::string GetName() const override { return "VoxelRender"; };

		UniformBuffer *GetUniformObject() const { return m_uniformObject; }
	};
}

#pragma once

#include <vector>
#include "Scenes/Objects/Component.hpp"
#include "Scenes/Objects/GameObject.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	/// <summary>
	/// Class used to render a GameObject as a entity.
	/// </summary>
	class F_EXPORT VoxelRender :
		public Component
	{
	private:
		DescriptorsHandler *m_descriptorSet;
		UniformHandler *m_uniformObject;
	public:
		VoxelRender();

		~VoxelRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler *uniformScene);

		std::string GetName() const override { return "VoxelRender"; };

		UniformHandler *GetUniformObject() const { return m_uniformObject; }
	};
}

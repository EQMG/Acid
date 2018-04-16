#pragma once

#include <vector>
#include "Scenes/Objects/Component.hpp"
#include "Scenes/Objects/GameObject.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	class F_EXPORT TerrainRender :
		public Component
	{
	private:
		UniformBuffer *m_uniformObject;
		DescriptorSet *m_descriptorSet;
	public:
		TerrainRender();

		~TerrainRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformBuffer *uniformScene);

		std::string GetName() const override { return "TerrainRender"; };

		UniformBuffer *GetUniformObject() const { return m_uniformObject; }
	};
}

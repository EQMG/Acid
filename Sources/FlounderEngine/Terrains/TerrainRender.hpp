#pragma once

#include <vector>
#include "Scenes/Objects/Component.hpp"
#include "Scenes/Objects/GameObject.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	class F_EXPORT TerrainRender :
		public Component
	{
	private:
		DescriptorsHandler *m_descriptorSet;
		UniformHandler *m_uniformObject;
	public:
		TerrainRender();

		~TerrainRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler *uniformScene);

		std::string GetName() const override { return "TerrainRender"; };

		UniformHandler *GetUniformObject() const { return m_uniformObject; }
	};
}

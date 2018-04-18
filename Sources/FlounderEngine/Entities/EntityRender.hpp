#pragma once

#include <vector>
#include "Scenes/Objects/Component.hpp"
#include "Scenes/Objects/GameObject.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	/// <summary>
	/// Class used to render a GameObject as a entity.
	/// </summary>
	class F_EXPORT EntityRender :
		public Component
	{
	private:
		DescriptorsHandler *m_descriptorSet;
		UniformHandler *m_uniformObject;
	public:
		EntityRender();

		~EntityRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, UniformHandler *uniformScene);

		std::string GetName() const override { return "EntityRender"; };

		UniformHandler *GetUniformObject() const { return m_uniformObject; }
	};
}

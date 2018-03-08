#pragma once

#include <vector>
#include "../Engine/Platform.hpp"
#include "../Objects/Component.hpp"
#include "../Objects/GameObject.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"

namespace Flounder
{
	/// <summary>
	/// Class used to render a GameObject as a entity.
	/// </summary>
	class F_EXPORT EntityRender :
		public Component
	{
	private:
		UniformBuffer *m_uniformObject;
	public:
		EntityRender();

		~EntityRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene);

		std::string GetName() const override { return "EntityRender"; };

		UniformBuffer *GetUniformObject() const { return m_uniformObject; }
	};
}

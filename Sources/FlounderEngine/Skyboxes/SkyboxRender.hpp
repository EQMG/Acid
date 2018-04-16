#pragma once

#include "Maths/Matrix4.hpp"
#include "Maths/Vector3.hpp"
#include "Scenes/Objects/Component.hpp"
#include "Scenes/Objects/GameObject.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Textures/Cubemap.hpp"

namespace Flounder
{
	class F_EXPORT SkyboxRender :
		public Component
	{
	private:
		UniformBuffer *m_uniformObject;
		DescriptorSet *m_descriptorSet;

		Cubemap *m_cubemap;
		bool m_enableFog;

		float m_blend;
	public:
		SkyboxRender(Cubemap *cubemap = nullptr, const bool &enableFog = true);

		~SkyboxRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformBuffer *uniformScene);

		std::string GetName() const override { return "SkyboxRender"; };

		Cubemap *GetCubemap() const { return m_cubemap; }

		void SetCubemap(Cubemap *cubemap) { m_cubemap = cubemap; }

		void TrySetCubemap(const std::string &filename);

		float GetBlend() const { return m_blend; }

		void SetBlend(const float blend) { m_blend = blend; }
	};
}

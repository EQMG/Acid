#pragma once

#include "../Maths/Matrix4.hpp"
#include "../Maths/Vector3.hpp"
#include "../Objects/Component.hpp"
#include "../Objects/GameObject.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"
#include "../Textures/Cubemap.hpp"

namespace Flounder
{
	class F_EXPORT SkyboxRender :
		public Component
	{
	private:
		UniformBuffer *m_uniformObject;

		Cubemap *m_cubemap;

		float m_blend;
	public:
		SkyboxRender(Cubemap *cubemap = nullptr);

		~SkyboxRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene);

		std::string GetName() const override { return "SkyboxRender"; };

		Cubemap *GetCubemap() const { return m_cubemap; }

		void SetCubemap(Cubemap *cubemap) { m_cubemap = cubemap; }

		void TrySetCubemap(const std::string &filename);

		float GetBlend() const { return m_blend; }

		void SetBlend(const float blend) { m_blend = blend; }
	};
}

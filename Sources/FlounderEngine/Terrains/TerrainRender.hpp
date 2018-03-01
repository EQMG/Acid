#pragma once

#include <vector>
#include "../Objects/Component.hpp"
#include "../Objects/GameObject.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	class F_EXPORT TerrainRender :
		public Component
	{
	private:
		UniformBuffer *m_uniformObject;
	public:
		static const int SIDE_LENGTH;
		static const std::vector<float> SQUARE_SIZES;
		static const std::vector<float> TEXTURE_SCALES;

		TerrainRender();

		~TerrainRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene);

		std::string GetName() const override { return "TerrainRender"; };

		UniformBuffer *GetUniformObject() const { return m_uniformObject; }
	};
}

#pragma once

#include "../Maths/Colour.hpp"
#include "../Objects/Component.hpp"
#include "../Objects/GameObject.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	/// <summary>
	/// Class used to render a GameObject as a water.
	/// </summary>
	class F_EXPORT WaterRender :
		public Component
	{
	private:
		UniformBuffer *m_uniformObject;

		Colour *m_colour;
	public:
		static const float SIDE_LENGTH;
		static const float SQUARE_SIZE;
		static const int VERTEX_COUNT;
		static const float TEXTURE_SCALE;

		static const Colour WATER_COLOUR;

		WaterRender();

		~WaterRender();

		void Update() override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene);

		std::string GetName() const override { return "WaterRender"; };

		UniformBuffer *GetUniformObject() const { return m_uniformObject; }

		Colour *GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) const { m_colour->Set(colour); }
	};
}

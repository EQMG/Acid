#pragma once

#include "Maths/Colour.hpp"
#include "Scenes/Objects/Component.hpp"
#include "Scenes/Objects/GameObject.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	/// <summary>
	/// Class used to render a GameObject as a water.
	/// </summary>
	class F_EXPORT WaterRender :
		public Component
	{
	private:
		DescriptorsHandler *m_descriptorSet;
		UniformHandler *m_uniformObject;

		Colour *m_colour;
	public:
		WaterRender();

		~WaterRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler *uniformScene);

		std::string GetName() const override { return "WaterRender"; };

		UniformHandler *GetUniformObject() const { return m_uniformObject; }

		Colour *GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) const { m_colour->Set(colour); }
	};
}

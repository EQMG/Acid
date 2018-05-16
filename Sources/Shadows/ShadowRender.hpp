#pragma once

#include <vector>
#include "Objects/Component.hpp"
#include "Objects/GameObject.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace fl
{
	/// <summary>
	/// Class used to render a GameObject as a shadow.
	/// </summary>
	class FL_EXPORT ShadowRender :
		public Component
	{
	private:
		DescriptorsHandler *m_descriptorSet;
		UniformHandler *m_uniformObject;
	public:
		ShadowRender();

		~ShadowRender();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		void CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler *uniformScene);

		std::string GetName() const override { return "ShadowRender"; };

		UniformHandler *GetUniformObject() const { return m_uniformObject; }
	};
}

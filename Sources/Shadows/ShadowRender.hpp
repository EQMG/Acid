#pragma once

#include <vector>
#include "Objects/GameObject.hpp"
#include "Objects/IComponent.hpp"
#include "Renderer/Handlers/DescriptorsHandler.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
	/// <summary>
	/// Class used to render a GameObject as a shadow.
	/// </summary>
	class ACID_EXPORT ShadowRender :
		public IComponent
	{
	private:
		DescriptorsHandler m_descriptorSet;
		UniformHandler m_uniformObject;
	public:
		ShadowRender();

		~ShadowRender();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		void CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler &uniformScene);

		UniformHandler GetUniformObject() const { return m_uniformObject; }
	};
}

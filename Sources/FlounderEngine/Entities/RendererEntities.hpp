#pragma once

#include "../Renderer/IRenderer.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"
#include "Entity.hpp"

namespace Flounder
{
	class F_HIDDEN EntityInstance
	{
	public:
		UniformBuffer *uniformBuffer;
		std::vector<UbosEntities::UboObject> objects;

		EntityInstance() :
			uniformBuffer(new UniformBuffer(1000 * sizeof(UbosEntities::UboObject))),
			objects(std::vector<UbosEntities::UboObject>())
		{
		}

		~EntityInstance()
		{
			delete uniformBuffer;
		}
	};

	class F_EXPORT RendererEntities :
		public IRenderer
	{
	private:
		UniformBuffer *m_uniformScene;
		Pipeline *m_pipeline;

		std::map<Model *, EntityInstance *> *m_instanceBuffers;
	public:
		RendererEntities(const int &subpass);

		~RendererEntities();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}

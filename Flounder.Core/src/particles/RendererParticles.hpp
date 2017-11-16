#pragma once

#include <vector>
#include "../renderer/IRenderer.hpp"
#include "Particles.hpp"

namespace Flounder
{
	class RendererParticles :
		public IRenderer
	{
	private:
		static const int MAX_INSTANCES;
		static const int INSTANCE_DATA_LENGTH;

		//int m_vaoID;
		//int m_vboID;
		//int m_vaoLength;

		int m_rendered;
	public:
		RendererParticles(const int &subpass);

		~RendererParticles();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void PrepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void PrepareInstance(Particle *particle, const ICamera &camera, std::vector<float> *vboData);

		void RenderInstances(ParticleType *particleType, std::vector<float> *vboData);

		void EndRendering();
	};
}

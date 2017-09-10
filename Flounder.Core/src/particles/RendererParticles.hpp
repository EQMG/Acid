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

		Shader *m_shader;

		//int m_vaoID;
		//int m_vboID;
		//int m_vaoLength;

		int m_rendered;
	public:
		RendererParticles();

		~RendererParticles();

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void PrepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void PrepareInstance(Particle *particle, const ICamera &camera, std::vector<float> *vboData);

		void RenderInstances(ParticleType *particleType, std::vector<float> *vboData);

		void EndRendering();
	};
}

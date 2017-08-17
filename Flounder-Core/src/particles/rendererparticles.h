#pragma once

#include <vector>

#include "../renderer/irenderer.h"

#include "particles.h"

namespace flounder
{
	class rendererparticles :
		public irenderer
	{
	private:
		static const int MAX_INSTANCES;
		static const int INSTANCE_DATA_LENGTH;

		shader *m_shader;

		//int m_vaoID;
		//int m_vboID;
		//int m_vaoLength;

		int m_rendered;
	public:
		rendererparticles();

		~rendererparticles();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

		void prepareInstance(particle *particle, const icamera &camera, std::vector<float> *vboData);

		void renderInstances(particletype *particleType, std::vector<float> *vboData);

		void endRendering();
	};
}

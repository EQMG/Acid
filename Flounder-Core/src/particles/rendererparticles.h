#pragma once

#include <vector>

#include "../loaders/loaders.h"
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

		GLuint m_vaoID;
		GLuint m_vboID;
		GLuint m_vaoLength;

		int m_rendered;
	public:
		rendererparticles();

		~rendererparticles();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

		void prepareTexturedModel(particletype *particleType);

		void prepareInstance(particle *particle, const icamera &camera, std::vector<GLfloat> *vboData);

		void renderInstances(std::vector<GLfloat> *vboData);

		void unbindTexturedModel();

		void endRendering();
	};
}

#pragma once

#include "../camera/camera.h"
#include "../framework/framework.h"
#include "../lights/fog.h"
#include "../maths/matrix4x4.h"
#include "../maths/vector3.h"
#include "../models/model.h"
#include "../textures/texture.h"

namespace flounder {
	class skybox : public imodule
	{
	private:
		float m_size;
		texture *m_texture;
		model *m_model;

		vector3 *m_rotation;
		matrix4x4 *m_modelMatrix;
		fog *m_fog;
		float m_blend;
	public:
		static skybox *get()
		{
			return (skybox*) framework::get()->getInstance("skybox");
		}

		skybox();

		~skybox();

		void loadTexture(texture *texture);

		void init();

		void update();

		inline float &getSize() { return m_size; }

		inline void setSize(const float size) { m_size = size; }

		inline texture *getTexture() { return m_texture; }

		inline model *getModel() { return m_model; }

		inline vector3 *getRotation() { return m_rotation; }

		inline void setRotation(vector3 *rotation) { m_rotation = rotation; }

		inline matrix4x4 *getModelMatrix() { return m_modelMatrix; }

		inline fog *getFog() { return m_fog; }

		inline void setFog(fog *fog) { m_fog = fog; }

		inline float &getBlend() { return m_blend; }

		inline void setBlend(const float blend) { m_blend = blend; }
	};
}

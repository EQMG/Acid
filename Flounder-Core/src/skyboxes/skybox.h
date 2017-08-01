#pragma once

#include "../camera/camera.h"
#include "../maths/matrix4x4.h"
#include "../maths/vector3.h"
#include "../models/model.h"
#include "../textures/texture.h"

namespace flounder
{
	class skybox
	{
	private:
		texture *m_texture;
		model *m_model;

		float m_size;
		float m_blend;

		vector3 *m_rotation;
		matrix4x4 *m_modelMatrix;
	public:
		skybox(texture *texture, model *model, const float &size);

		~skybox();

		void update();

		inline texture *getTexture() const { return m_texture; }

		inline void setTexture(texture *texture) { m_texture = texture; }

		inline model *getModel() const { return m_model; }

		inline void setModel(model *model) { m_model = model; }

		inline float getSize() const { return m_size; }

		inline void setSize(const float size) { m_size = size; }

		inline float getBlend() const { return m_blend; }

		inline void setBlend(const float blend) { m_blend = blend; }

		inline vector3 *getRotation() const { return m_rotation; }

		inline void setRotation(vector3 *rotation) { m_rotation = rotation; }

		inline matrix4x4 *getModelMatrix() const { return m_modelMatrix; }
	};
}

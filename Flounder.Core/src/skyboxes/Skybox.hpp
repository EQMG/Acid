#pragma once

#include "../camera/camera.hpp"
#include "../maths/Matrix4.hpp"
#include "../maths/Vector3.hpp"
#include "../models/model.hpp"
#include "../textures/texture.hpp"

namespace Flounder
{
	class skybox
	{
	private:
		texture *m_texture;
		model *m_model;

		float m_size;
		float m_blend;

		Vector3 *m_rotation;
		Matrix4 *m_modelMatrix;
	public:
		skybox(texture *texture, model *model, const float &size);

		~skybox();

		void update();

		texture *getTexture() const { return m_texture; }

		void setTexture(texture *texture) { m_texture = texture; }

		model *getModel() const { return m_model; }

		void setModel(model *model) { m_model = model; }

		float getSize() const { return m_size; }

		void setSize(const float size) { m_size = size; }

		float getBlend() const { return m_blend; }

		void setBlend(const float blend) { m_blend = blend; }

		Vector3 *getRotation() const { return m_rotation; }

		void setRotation(Vector3 *rotation) { m_rotation = rotation; }

		Matrix4 *getModelMatrix() const { return m_modelMatrix; }
	};
}

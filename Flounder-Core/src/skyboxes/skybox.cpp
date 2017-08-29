#include "skybox.hpp"

namespace flounder
{
	skybox::skybox(texture *texture, model *model, const float &size)
	{
		m_texture = texture;
		m_model = model;

		m_size = size;
		m_blend = 1.0f;

		m_rotation = new vector3();
		m_modelMatrix = new matrix4x4();
	}

	skybox::~skybox()
	{
		delete m_texture;
		delete m_model;

		delete m_rotation;
		delete m_modelMatrix;
	}

	void skybox::update()
	{
		matrix4x4::transformationMatrix(*camera::get()->getCamera()->getPosition(), *m_rotation, m_size, m_modelMatrix);
	}
}

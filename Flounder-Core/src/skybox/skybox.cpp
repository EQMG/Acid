#include "skybox.h"

namespace flounder 
{
	skybox::skybox() : 
		imodule()
	{
		m_size = 250.0f;
		m_texture = NULL;
		m_model = NULL;

		m_rotation = new vector3();
		m_modelMatrix = new matrix4x4();
		m_fog = new fog(new colour(), 0.001f, 2.0f, -1.28f, 51.2f);
		m_blend = 1.0f;
	}

	skybox::~skybox()
	{
		delete m_texture;
		delete m_model;

		delete m_rotation;
		delete m_modelMatrix;
		delete m_fog;
	}

	void skybox::load(texture *texture, model *model)
	{
		m_texture = texture;
		m_model = model;
	}

	void skybox::init()
	{
	}

	void skybox::update()
	{
		if (camera::get()->getCamera() != NULL)
		{
			matrix4x4::transformationMatrix(*camera::get()->getCamera()->getPosition(), *m_rotation, m_size, m_modelMatrix);
		}
	}
}

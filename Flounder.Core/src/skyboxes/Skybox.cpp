#include "Skybox.hpp"

#include "../camera/Camera.hpp"

namespace Flounder
{
	Skybox::Skybox(Texture *texture, Model *model, const float &size) :
		m_texture(texture),
		m_model(model),
		m_size(size),
		m_blend(1.0f),
		m_rotation(new Vector3()),
		m_modelMatrix(new Matrix4())
	{
	}

	Skybox::~Skybox()
	{
		delete m_texture;
		delete m_model;

		delete m_rotation;
		delete m_modelMatrix;
	}

	void Skybox::Update()
	{
		Matrix4::TransformationMatrix(*Camera::Get()->GetCamera()->GetPosition(), *m_rotation, m_size, m_modelMatrix);
	}
}

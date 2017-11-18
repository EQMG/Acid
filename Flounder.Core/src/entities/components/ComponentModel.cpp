#include "ComponentModel.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentModel::ComponentModel(Model *model, Texture *diffuse) :
		IComponent(),
		m_model(model),
		m_textureDiffuse(diffuse)
	{
	}

	ComponentModel::~ComponentModel()
	{
	}

	void ComponentModel::Update()
	{
	}

	void ComponentModel::CmdRender(EntityRender *entityRender)
	{
		entityRender->descriptorWrites.push_back(m_textureDiffuse->GetWriteDescriptor(2, entityRender->descriptorSet));
		entityRender->model = m_model;
	}
}

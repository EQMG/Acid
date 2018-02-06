#include <Models/Shapes/Sphere.hpp>
#include "ComponentModel.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	static Model *TempModel(const std::string &filename)
	{
		auto split = FormatString::Split(filename, "_");
		if (!split.empty() && split.at(0) == "Sphere")
		{
			return Sphere::Resource(filename);
		}
		return Model::Resource(filename);
	}

	ComponentModel::ComponentModel(Model *model, Texture *diffuse) :
		IComponent(),
		m_model(model),
		m_textureDiffuse(diffuse)
	{
	}

	ComponentModel::ComponentModel(ComponentPrefab *prefab) :
		IComponent(),
		m_model(TempModel(prefab->GetString(0))),
		m_textureDiffuse(Texture::Resource(prefab->GetString(1)))
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

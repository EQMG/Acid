#include "ComponentModel.hpp"

#include "../Entity.hpp"
#include "../../Models/Shapes/Sphere.hpp"

namespace Flounder
{
	static Model *DecodeModelType(const std::string &filename)
	{
		auto split = FormatString::Split(filename, "_");
		if (!split.empty() && split.at(0) == "Sphere")
		{
			return Sphere::Resource(filename);
		}
		return Model::Resource(filename);
	}

	ComponentModel::ComponentModel(Model *model) :
		IComponent(),
		m_model(model)
	{
	}

	ComponentModel::ComponentModel(ComponentPrefab *prefab) :
		IComponent(),
		m_model(DecodeModelType(prefab->GetString(0)))
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
		entityRender->model = m_model;
	}

	void ComponentModel::Save(ComponentPrefab *prefab)
	{
		prefab->SetString(0, m_model->GetFilename());
	}
}

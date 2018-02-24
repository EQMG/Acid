#include "Mesh.hpp"

#include "../Models/Shapes/ShapeSphere.hpp"
#include "../Models/Shapes/ShapeCube.hpp"
#include "../Models/Shapes/ShapeRectangle.hpp"

namespace Flounder
{
	Mesh::Mesh(Model *model) :
		Component(),
		m_model(model)
	{
		Link<std::string>(0, "Model", LINK_GET_RES(GetModel()), LINK_SET(std::string, TrySetModel(v)));
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Update()
	{
		/*if (m_model == nullptr || m_model->GetAabb() == nullptr)
		{
			return;
		}

		auto aabb = GetGameObject()->GetComponent<Aabb>();

		if (aabb != m_model->GetAabb())
		{
			GetGameObject()->RemoveComponent(aabb);
			GetGameObject()->AddComponent(m_model->GetAabb());
		}*/
	}

	void Mesh::TrySetModel(const std::string &filename)
	{
		if (filename.empty() || filename == "nullptr")
		{
			return;
		}

		auto split = FormatString::Split(filename, "_");

		if (!split.empty() && split.at(0) == "Sphere")
		{
			m_model = ShapeSphere::Resource(filename);
			return;
		}
		
		if (!split.empty() && split.at(0) == "Cube")
		{
			m_model = ShapeCube::Resource(filename);
			return;
		}
		
		if (!split.empty() && split.at(0) == "Rectangle")
		{
			m_model = ShapeRectangle::Resource(filename);
			return;
		}

		m_model = Model::Resource(filename);
	}
}

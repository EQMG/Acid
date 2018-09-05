#include "Mesh.hpp"

#include "Helpers/FileSystem.hpp"
#include "Models/Shapes/ModelCube.hpp"
#include "Models/Shapes/ModelCylinder.hpp"
#include "Models/Shapes/ModelDisk.hpp"
#include "Models/Shapes/ModelRectangle.hpp"
#include "Models/Shapes/ModelSphere.hpp"
#include "Models/Obj/ModelObj.hpp"

namespace acid
{
	Mesh::Mesh(const std::shared_ptr<Model> &model) :
		m_model(model)
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Start()
	{
	}

	void Mesh::Update()
	{
		// TODO
		/*if (m_model == nullptr || m_model->GetAabb() == nullptr)
		{
			return;
		}

		auto aabb = GetGameObject()->GetComponent<BoundingBox>();

		if (aabb != m_model->GetAabb())
		{
			GetGameObject()->RemoveComponent(aabb);
			GetGameObject()->AddComponent(m_model->GetAabb());
		}*/
	}

	void Mesh::Decode(const Metadata &metadata)
	{
		TrySetModel(metadata.GetChild<std::string>("Model"));
	}

	void Mesh::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Model", m_model == nullptr ? "" : m_model->GetFilename());
	}

	void Mesh::TrySetModel(const std::string &filename)
	{
		if (filename.empty())
		{
			return;
		}

		// TODO: Modularize.
		auto split = String::Split(filename, "_");

		if (!split.empty() && split[0] == "Cube")
		{
			m_model = ModelCube::Resource(filename);
			return;
		}

		if (!split.empty() && split[0] == "Cylinder")
		{
			m_model = ModelCylinder::Resource(filename);
			return;
		}

		if (!split.empty() && split[0] == "Disk")
		{
			m_model = ModelDisk::Resource(filename);
			return;
		}

		if (!split.empty() && split[0] == "Rectangle")
		{
			m_model = ModelRectangle::Resource(filename);
			return;
		}

		if (!split.empty() && split[0] == "Sphere")
		{
			m_model = ModelSphere::Resource(filename);
			return;
		}

		if (FileSystem::FindExt(filename) == "obj")
		{
			m_model = ModelObj::Resource(filename);
			return;
		}

		Log::Error("Could not determine mesh model type: '%s'\n", filename.c_str());
	}
}

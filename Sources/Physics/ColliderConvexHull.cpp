#include "ColliderConvexHull.hpp"

#include "Scenes/Scenes.hpp"
#include "Meshes/Mesh.hpp"

namespace acid
{
	ColliderConvexHull::ColliderConvexHull(const std::vector<float> &pointCloud) :
		m_shape(nullptr),
		m_model(nullptr),
		m_points(0)
	{
		if (pointCloud.empty())
		{
			return;
		}

		SetPointCloud(pointCloud);
	}

	ColliderConvexHull::~ColliderConvexHull()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ColliderConvexHull::Start()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh != nullptr && mesh->GetModel() != nullptr)
		{
			SetPointCloud(mesh->GetModel()->GetPointCloud());
		}
	}

	void ColliderConvexHull::Update()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() != nullptr)
		{
			return;
		}

		if (m_shape != nullptr && m_model == nullptr)
		{
			if (mesh->GetModel()->GetPointCloud().size() / 3 == m_points)
			{
				m_model = mesh->GetModel();
				return;
			}
		}

		if (m_model != mesh->GetModel())
		{
			m_model == mesh->GetModel();
			SetPointCloud(m_model->GetPointCloud());
		}
	}

	void ColliderConvexHull::Load(LoadedValue *value)
	{
	}

	void ColliderConvexHull::Write(LoadedValue *destination)
	{
	}

	void ColliderConvexHull::SetPointCloud(const std::vector<float> &pointCloud)
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;

		if (pointCloud.empty())
		{
			return;
		}

		m_shape = new btConvexHullShape(pointCloud.data(), pointCloud.size() / 3, sizeof(float));
		m_shape->optimizeConvexHull();
		m_shape->initializePolyhedralFeatures();
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
		m_points = pointCloud.size() / 3;
	}
}

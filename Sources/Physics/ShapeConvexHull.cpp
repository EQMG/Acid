#include "ShapeConvexHull.hpp"

#include "Scenes/Scenes.hpp"
#include "Meshes/Mesh.hpp"

namespace fl
{
	ShapeConvexHull::ShapeConvexHull(const std::vector<float> &pointCloud) :
		m_shape(nullptr)
	{
		if (pointCloud.empty())
		{
			return;
		}

		SetPointCloud(pointCloud);
	}

	ShapeConvexHull::~ShapeConvexHull()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeConvexHull::Start()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh != nullptr && mesh->GetModel() != nullptr)
		{
			SetPointCloud(mesh->GetModel()->GetPointCloud());
		}
	}

	void ShapeConvexHull::Update()
	{
		// TODO: Update point cloud on components change.
	}

	void ShapeConvexHull::Load(LoadedValue *value)
	{
	}

	void ShapeConvexHull::Write(LoadedValue *destination)
	{
	}

	void ShapeConvexHull::SetPointCloud(const std::vector<float> &pointCloud)
	{
		// TODO
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;

		if (pointCloud.empty())
		{
			return;
		}

		m_shape = new btConvexHullShape(pointCloud.data(), pointCloud.size() / 3, sizeof(btScalar));
		m_shape->optimizeConvexHull();
		m_shape->initializePolyhedralFeatures();
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}
}

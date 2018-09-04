#include "ColliderConvexHull.hpp"

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include "Meshes/Mesh.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderConvexHull::ColliderConvexHull(const std::vector<float> &pointCloud) :
		Collider(),
		m_shape(nullptr),
		m_model(nullptr),
		m_points(0)
	{
		if (pointCloud.empty())
		{
			return;
		}

		Initialize(pointCloud);
	}

	ColliderConvexHull::~ColliderConvexHull()
	{
		delete m_shape;
	}

	void ColliderConvexHull::Start()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>(true);

		if (mesh != nullptr && mesh->GetModel() != nullptr)
		{
			Initialize(mesh->GetModel()->GetPointCloud());
		}
	}

	void ColliderConvexHull::Update()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>(true);

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
			Initialize(m_model->GetPointCloud());
		}
	}

	void ColliderConvexHull::Decode(const Node &node)
	{
	}

	void ColliderConvexHull::Encode(Node &node) const
	{
	}

	btCollisionShape *ColliderConvexHull::GetCollisionShape() const
	{
		return m_shape;
	}

	void ColliderConvexHull::Initialize(const std::vector<float> &pointCloud)
	{
		delete m_shape;
		m_points = 0;

		if (pointCloud.empty())
		{
			return;
		}

		m_shape = new btConvexHullShape(pointCloud.data(), pointCloud.size() / 3, sizeof(float));
		m_shape->optimizeConvexHull();
		m_shape->initializePolyhedralFeatures();
		m_points = pointCloud.size() / 3;
	}
}

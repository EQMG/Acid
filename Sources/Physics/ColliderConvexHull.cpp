#include "ColliderConvexHull.hpp"

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include "Meshes/Mesh.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderConvexHull::ColliderConvexHull(const std::vector<float> &pointCloud, const Transform &localTransform) :
		Collider(localTransform),
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

	void ColliderConvexHull::Decode(const Metadata &metadata)
	{
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
	}

	void ColliderConvexHull::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
	}

	btCollisionShape *ColliderConvexHull::GetCollisionShape() const
	{
		return m_shape.get();
	}

	void ColliderConvexHull::Initialize(const std::vector<float> &pointCloud)
	{
		if (pointCloud.empty())
		{
			return;
		}

		m_shape = std::make_unique<btConvexHullShape>(pointCloud.data(), static_cast<int32_t>(pointCloud.size() / 3), sizeof(float));
		m_shape->optimizeConvexHull();
		m_shape->initializePolyhedralFeatures();
		m_points = static_cast<uint32_t>(pointCloud.size() / 3);
	}
}

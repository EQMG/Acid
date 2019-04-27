#include "ColliderConvexHull.hpp"

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include "Scenes/Entity.hpp"
#include "Meshes/Mesh.hpp"

namespace acid
{
ColliderConvexHull::ColliderConvexHull(const std::vector<float> &pointCloud, const Transform &localTransform) :
	Collider(localTransform),
	m_shape(nullptr),
	m_model(nullptr),
	m_pointCount(0)
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
	auto mesh = GetParent()->GetComponent<Mesh>(true);

	if (mesh != nullptr && mesh->GetModel() != nullptr)
	{
		Initialize(mesh->GetModel()->GetPointCloud());
	}
}

void ColliderConvexHull::Update()
{
	auto mesh = GetParent()->GetComponent<Mesh>(true);

	if (mesh == nullptr || mesh->GetModel() != nullptr)
	{
		return;
	}

	if (m_shape != nullptr && m_model == nullptr)
	{
		if (mesh->GetModel()->GetPointCloud().size() / 3 == m_pointCount)
		{
			m_model = mesh->GetModel();
			return;
		}
	}

	if (m_model != mesh->GetModel())
	{
		m_model = mesh->GetModel();
		Initialize(m_model->GetPointCloud());
	}
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

	m_shape = std::make_unique<btConvexHullShape>(pointCloud.data(), static_cast<int32_t>(pointCloud.size() / 3), static_cast<int32_t>(sizeof(float)));
	m_shape->optimizeConvexHull();
	m_shape->initializePolyhedralFeatures();
	m_pointCount = static_cast<uint32_t>(pointCloud.size() / 3);
}

const Metadata& operator>>(const Metadata& metadata, ColliderConvexHull& collider)
{
	metadata.GetChild("Local Transform", collider.m_localTransform);
	return metadata;
}

Metadata& operator<<(Metadata& metadata, const ColliderConvexHull& collider)
{
	metadata.SetChild("Local Transform", collider.m_localTransform);
	return metadata;
}
}

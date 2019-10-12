#include "ColliderConvexHull.hpp"

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include "Meshes/Mesh.hpp"
#include "Physics/CollisionObject.hpp"

namespace acid {
bool ColliderConvexHull::registered = Register("convexHull");

ColliderConvexHull::ColliderConvexHull(const std::vector<float> &pointCloud, const Transform &localTransform) /*:
	Collider(localTransform)*/ {
	m_localTransform = localTransform;
	if (pointCloud.empty()) {
		return;
	}

	SetPointCount(pointCloud);
}

ColliderConvexHull::~ColliderConvexHull() {
}

/*void ColliderConvexHull::Start() {
	auto mesh = GetEntity()->GetComponent<Mesh>(true);

	if (mesh && mesh->GetModel()) {
		SetPointCount(mesh->GetModel()->GetPointCloud());
	}
}

void ColliderConvexHull::Update() {
	// TODO
	auto mesh = GetEntity()->GetComponent<Mesh>(true);
	if (!mesh || mesh->GetModel())
		return;

	if (m_shape && !m_model) {
		if (mesh->GetModel()->GetPointCloud().size() / 3 == m_pointCount) {
			m_model = mesh->GetModel();
			return;
		}
	}

	if (m_model != mesh->GetModel()) {
		m_model = mesh->GetModel();
		SetPointCount(m_model->GetPointCloud());
	}
}*/

btCollisionShape *ColliderConvexHull::GetCollisionShape() const {
	return m_shape.get();
}

void ColliderConvexHull::SetPointCount(const std::vector<float> &pointCloud) {
	if (pointCloud.empty()) return;

	m_shape = std::make_unique<btConvexHullShape>(pointCloud.data(), static_cast<int32_t>(pointCloud.size() / 3), static_cast<int32_t>(sizeof(float)));
	m_shape->optimizeConvexHull();
	m_shape->initializePolyhedralFeatures();
	m_pointCount = static_cast<uint32_t>(pointCloud.size() / 3);
}

const Node &operator>>(const Node &node, ColliderConvexHull &collider) {
	node["localTransform"].Get(collider.m_localTransform);
	return node;
}

Node &operator<<(Node &node, const ColliderConvexHull &collider) {
	node["localTransform"].Set(collider.m_localTransform);
	return node;
}
}

#include "ColliderConvexHull.hpp"

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include "Meshes/Mesh.hpp"

namespace acid {
ColliderConvexHull::ColliderConvexHull(const std::vector<float> &pointCloud, const Transform &localTransform) /*:
	Collider(localTransform)*/ {
	this->localTransform = localTransform;
	if (!pointCloud.empty())
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

	if (shape && !model) {
		if (mesh->GetModel()->GetPointCloud().size() / 3 == pointCount) {
			model = mesh->GetModel();
			return;
		}
	}

	if (model != mesh->GetModel()) {
		model = mesh->GetModel();
		SetPointCount(model->GetPointCloud());
	}
}*/

btCollisionShape *ColliderConvexHull::GetCollisionShape() const {
	return shape.get();
}

void ColliderConvexHull::SetPointCount(const std::vector<float> &pointCloud) {
	if (pointCloud.empty()) return;

	shape = std::make_unique<btConvexHullShape>(pointCloud.data(), static_cast<int32_t>(pointCloud.size() / 3), static_cast<int32_t>(sizeof(float)));
	shape->optimizeConvexHull();
	shape->initializePolyhedralFeatures();
	pointCount = static_cast<uint32_t>(pointCloud.size() / 3);
}

const Node &operator>>(const Node &node, ColliderConvexHull &collider) {
	node["localTransform"].Get(collider.localTransform);
	return node;
}

Node &operator<<(Node &node, const ColliderConvexHull &collider) {
	node["localTransform"].Set(collider.localTransform);
	return node;
}
}

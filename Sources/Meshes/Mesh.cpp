#include "Mesh.hpp"

namespace acid {
Mesh::Mesh(const std::shared_ptr<Model> &model) :
	m_model(model) {
}

void Mesh::Start() {
}

void Mesh::Update() {
}

const Node &operator>>(const Node &node, Mesh &mesh) {
	node["model"].Get(mesh.m_model);
	return node;
}

Node &operator<<(Node &node, const Mesh &mesh) {
	node["model"].Set(mesh.m_model);
	return node;
}
}

#include "Mesh.hpp"

namespace acid
{
Mesh::Mesh(const std::shared_ptr<Model>& model) : m_model(model) {}

void Mesh::Start() {}

void Mesh::Update() {}

void Mesh::Decode(const Metadata& metadata)
{
	metadata.GetResource("Model", m_model);
}

void Mesh::Encode(Metadata& metadata) const
{
	metadata.SetResource("Model", m_model);
}
}

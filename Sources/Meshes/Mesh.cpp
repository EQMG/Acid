#include "Mesh.hpp"

namespace acid
{
Mesh::Mesh(const std::shared_ptr<Model> &model) :
	m_model{model}
{
}

void Mesh::Start()
{
}

void Mesh::Update()
{
}

const Metadata &operator>>(const Metadata &metadata, Mesh &mesh)
{
	metadata.GetResource("model", mesh.m_model);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Mesh &mesh)
{
	metadata.SetResource("model", mesh.m_model);
	return metadata;
}
}

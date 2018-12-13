#include "Mesh.hpp"

namespace acid
{
	Mesh::Mesh(const std::shared_ptr<Model> &model) :
		m_model(model)
	{
	}

	void Mesh::Start()
	{
	}

	void Mesh::Update()
	{
	}

	void Mesh::Decode(const Metadata &metadata)
	{
		TrySetModel(metadata.GetChild<std::string>("Model"));
	}

	void Mesh::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Model", m_model == nullptr ? "" : m_model->GetFilename());
	}

	void Mesh::TrySetModel(const std::string &filename)
	{
		m_model = Model::Resource(filename);
	}
}

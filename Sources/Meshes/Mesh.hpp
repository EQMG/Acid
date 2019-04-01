#pragma once

#include "Models/Model.hpp"
#include "Models/VertexModel.hpp"
#include "Scenes/Component.hpp"

namespace acid
{
/**
 * @brief Component that represents a model/mesh.
 */
class ACID_EXPORT Mesh :
	public Component
{
public:
	/**
	 * Creates a new mesh component with a model.
	 * @param model The initial model to use in this mesh.
	 */
	explicit Mesh(const std::shared_ptr<Model> &model = nullptr);

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	const std::shared_ptr<Model> &GetModel() const { return m_model; }

	static Shader::VertexInput GetVertexInput(const uint32_t &binding = 0) { return VertexModel::GetVertexInput(binding); }

	void SetModel(const std::shared_ptr<Model> &model) { m_model = model; }

private:
	std::shared_ptr<Model> m_model;
};
}

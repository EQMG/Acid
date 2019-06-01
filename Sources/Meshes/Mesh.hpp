#pragma once

#include "Models/Model.hpp"
#include "Models/VertexDefault.hpp"
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

	virtual const std::shared_ptr<Model> &GetModel() const { return m_model; }

	virtual Shader::VertexInput GetVertexInput(const uint32_t &binding = 0) const { return VertexDefault::GetVertexInput(binding); }

	virtual void SetModel(const std::shared_ptr<Model> &model) { m_model = model; }

	friend const Metadata &operator>>(const Metadata &metadata, Mesh &mesh);

	friend Metadata &operator<<(Metadata &metadata, const Mesh &mesh);

private:
	std::shared_ptr<Model> m_model;
};
}

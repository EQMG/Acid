#pragma once

#include "Models/Model.hpp"
#include "Models/VertexDefault.hpp"
#include "Scenes/Component.hpp"

namespace acid {
/**
 * @brief Component that represents a model/mesh.
 */
class ACID_EXPORT Mesh : public Component::Registrar<Mesh> {
public:
	/**
	 * Creates a new mesh component with a model.
	 * @param model The initial model to use in this mesh.
	 */
	explicit Mesh(const std::shared_ptr<Model> &model = nullptr);

	void Start() override;
	void Update() override;

	static Shader::VertexInput GetVertexInput(uint32_t binding = 0) { return VertexDefault::GetVertexInput(binding); }

	const std::shared_ptr<Model> &GetModel() const { return m_model; }
	void SetModel(const std::shared_ptr<Model> &model) { m_model = model; }

	friend const Node &operator>>(const Node &node, Mesh &mesh);
	friend Node &operator<<(Node &node, const Mesh &mesh);

private:
	static bool registered;

	std::shared_ptr<Model> m_model;
};
}

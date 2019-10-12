#pragma once

#include "Models/Model.hpp"
#include "Models/VertexDefault.hpp"
#include "Scenes/Component.hpp"
#include "Materials/Material.hpp"

namespace acid {
/**
 * @brief Component that represents a model/mesh.
 */
class ACID_EXPORT Mesh : public Component::Registrar<Mesh> {
public:
	/**
	 * Creates a new mesh component.
	 * @param model The model to use in this mesh.
	 * @param material The material to render this mesh with.
	 */
	explicit Mesh(std::shared_ptr<Model> model = nullptr, std::unique_ptr<Material> &&material = nullptr);

	void Start() override;
	void Update() override;

	bool CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline::Stage &pipelineStage);

	static Shader::VertexInput GetVertexInput(uint32_t binding = 0) { return VertexDefault::GetVertexInput(binding); }

	const std::shared_ptr<Model> &GetModel() const { return m_model; }
	void SetModel(const std::shared_ptr<Model> &model) { m_model = model; }

	const std::unique_ptr<Material> &GetMaterial() const { return m_material; }
	void SetMaterial(std::unique_ptr<Material> &&material);

	bool operator<(const Mesh &other) const;
	bool operator>(const Mesh &other) const;

	friend const Node &operator>>(const Node &node, Mesh &mesh);
	friend Node &operator<<(Node &node, const Mesh &mesh);

private:
	static bool registered;

	std::shared_ptr<Model> m_model;
	std::unique_ptr<Material> m_material;

	DescriptorsHandler m_descriptorSet;
	UniformHandler m_uniformObject;
};
}

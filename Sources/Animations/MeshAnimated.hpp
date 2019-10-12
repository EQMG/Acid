#pragma once

#include "Materials/Material.hpp"
#include "Models/Model.hpp"
#include "Scenes/Component.hpp"
#include "Graphics/Buffers/StorageHandler.hpp"
#include "Geometry/VertexAnimated.hpp"
#include "Animator.hpp"

namespace acid {
/**
 * @brief Class that represents an animated armature with a skin mesh.
 **/
class ACID_EXPORT MeshAnimated : public Component::Registrar<MeshAnimated> {
public:
	/**
	 * Creates a new animated mesh component.
	 * @param filename The file to load the model and animation from.
	 * @param material The material to render this mesh with.
	 */
	explicit MeshAnimated(std::filesystem::path filename = "", std::unique_ptr<Material> &&material = nullptr);

	void Start() override;
	void Update() override;

	bool CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline::Stage &pipelineStage);

	static Shader::VertexInput GetVertexInput(uint32_t binding = 0) { return VertexAnimated::GetVertexInput(binding); }

	const std::shared_ptr<Model> &GetModel() const { return m_model; }
	void SetModel(const std::shared_ptr<Model> &model) { m_model = model; }

	const std::unique_ptr<Material> &GetMaterial() const { return m_material; }
	void SetMaterial(std::unique_ptr<Material> &&material);

	StorageHandler &GetStorageAnimation() { return m_storageAnimation; }

	friend const Node &operator>>(const Node &node, MeshAnimated &meshAnimated);
	friend Node &operator<<(Node &node, const MeshAnimated &meshAnimated);

	static constexpr uint32_t MaxJoints = 50;
	static constexpr uint32_t MaxWeights = 3;

private:
	static bool registered;

	std::shared_ptr<Model> m_model;
	std::unique_ptr<Material> m_material;
	
	std::filesystem::path m_filename;
	Animator m_animator;
	Joint m_headJoint;
	
	std::unique_ptr<Animation> m_animation;

	DescriptorsHandler m_descriptorSet;
	UniformHandler m_uniformObject;
	StorageHandler m_storageAnimation;
};
}

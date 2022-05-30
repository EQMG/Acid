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
 */
class ACID_EXPORT AnimatedMesh : public Component::Registrar<AnimatedMesh> {
	inline static const bool Registered = Register("animatedMesh");
public:
	/**
	 * Creates a new animated mesh component.
	 * @param filename The file to load the model and animation from.
	 * @param material The material to render this mesh with.
	 */
	explicit AnimatedMesh(std::filesystem::path filename = "", std::unique_ptr<Material> &&material = nullptr);

	void Start() override;
	void Update() override;

	bool CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline::Stage &pipelineStage);

	static Shader::VertexInput GetVertexInput(uint32_t binding = 0) { return VertexAnimated::GetVertexInput(binding); }

	const std::shared_ptr<Model> &GetModel() const { return model; }
	void SetModel(const std::shared_ptr<Model> &model) { this->model = model; }

	const std::unique_ptr<Material> &GetMaterial() const { return material; }
	void SetMaterial(std::unique_ptr<Material> &&material);

	StorageHandler &GetStorageAnimation() { return storageAnimation; }

	friend const Node &operator>>(const Node &node, AnimatedMesh &animatedMesh);
	friend Node &operator<<(Node &node, const AnimatedMesh &animatedMesh);

	constexpr static uint32_t MaxJoints = 50;
	constexpr static uint32_t MaxWeights = 3;

private:
	std::shared_ptr<Model> model;
	std::unique_ptr<Material> material;
	
	std::filesystem::path filename;
	Animator animator;
	Joint headJoint;
	
	std::unique_ptr<Animation> animation;

	DescriptorsHandler descriptorSet;
	UniformHandler uniformObject;
	StorageHandler storageAnimation;
};
}

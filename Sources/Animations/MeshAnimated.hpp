#pragma once

#include "Maths/Matrix4.hpp"
#include "Models/Model.hpp"
#include "Scenes/Component.hpp"
#include "Graphics/Buffers/StorageHandler.hpp"
#include "Animation/AnimationLoader.hpp"
#include "Geometry/GeometryLoader.hpp"
#include "Geometry/VertexAnimated.hpp"
#include "Animator.hpp"

namespace acid
{
/**
 * @brief Class that represents an animated armature with a skin mesh.
 **/
class ACID_EXPORT MeshAnimated :
	public Component
{
public:
	explicit MeshAnimated(std::filesystem::path filename = "");

	void Start() override;
		
	void Update() override;

	static Shader::VertexInput GetVertexInput(const uint32_t &binding = 0) { return VertexAnimated::GetVertexInput(binding); }

	const std::shared_ptr<Model> &GetModel() const { return m_model; }

	void SetModel(const std::shared_ptr<Model> &model) { m_model = model; }

	StorageHandler &GetStorgeAnimation() { return m_storageAnimation; }

	friend const Metadata &operator>>(const Metadata &metadata, MeshAnimated &meshAnimated);

	friend Metadata &operator<<(Metadata &metadata, const MeshAnimated &meshAnimated);

	static constexpr uint32_t MaxJoints{50};
	static constexpr uint32_t MaxWeights{3};

private:
	static std::unique_ptr<Joint> CreateJoints(const JointData &data);

	Animator m_animator;

	std::filesystem::path m_filename;
	std::shared_ptr<Model> m_model;
	std::unique_ptr<Joint> m_headJoint;
	std::unique_ptr<Animation> m_animation;

	StorageHandler m_storageAnimation;
};
}

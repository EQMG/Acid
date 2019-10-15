#include "MeshAnimated.hpp"

#include "Maths/Maths.hpp"
#include "Files/File.hpp"
#include "Maths/Matrix4.hpp"
#include "Scenes/Entity.inl"
#include "Animation/AnimationLoader.hpp"
#include "Skeleton/SkeletonLoader.hpp"
#include "Skin/SkinLoader.hpp"
#include "Geometry/GeometryLoader.hpp"
#include "Maths/Transform.hpp"

namespace acid {
bool MeshAnimated::registered = Register("meshAnimated");

MeshAnimated::MeshAnimated(std::filesystem::path filename, std::unique_ptr<Material> &&material) :
	m_material(std::move(material)),
	m_filename(std::move(filename)) {
}

void MeshAnimated::Start() {
	if (m_material)
		m_material->Start(GetVertexInput(), true);

	if (m_filename.empty())
		return;

	//File file(m_filename, std::make_unique<Xml>("COLLADA"));
	//file.Load();
	//auto &fileNode = *file.GetNode();
	File file(m_filename);
	file.Load();
	auto fileNode = (*file.GetNode())["COLLADA"];

	// Because in Blender z is up, but Acid is y up. A correction must be applied to positions and normals.
	static const auto Correction = Matrix4().Rotate(Maths::Radians(-90.0f), Vector3f::Right);

	SkinLoader skinLoader(fileNode["library_controllers"], MaxWeights);
	SkeletonLoader skeletonLoader(fileNode["library_visual_scenes"], skinLoader.GetJointOrder(), Correction);
	GeometryLoader geometryLoader(fileNode["library_geometries"], skinLoader.GetVertexWeights(), Correction);

	m_model = std::make_shared<Model>(geometryLoader.GetVertices(), geometryLoader.GetIndices());
	m_headJoint = skeletonLoader.GetHeadJoint();

	AnimationLoader animationLoader(fileNode["library_animations"], fileNode["library_visual_scenes"], Correction);

	m_animation = std::make_unique<Animation>(animationLoader.GetLengthSeconds(), animationLoader.GetKeyframes());
	m_animator.DoAnimation(m_animation.get());

/*#if defined(ACID_DEBUG)
	{
		File fileModel("Animation/Model.json");
		(*fileModel.GetNode())["vertices"].Set(m_model->GetVertices<VertexAnimated>());
		(*fileModel.GetNode())["indices"].Set(m_model->GetIndices());
		fileModel.Write(Node::Format::Beautified);
	}
	{
		File fileJoints("Animation/Joints.json");
		*fileJoints.GetNode() << m_headJoint;
		fileJoints.Write(Node::Format::Beautified);
	}
	{
		File fileAnimation0("Animation/Animation0.json");
		*fileAnimation0.GetNode() << *m_animation;
		fileAnimation0.Write(Node::Format::Beautified);
	}
#endif*/
}

void MeshAnimated::Update() {
	if (m_material) {
		auto transform = GetEntity()->GetComponent<Transform>();
		m_material->PushUniforms(m_uniformObject, transform);
	}
	
	std::vector<Matrix4> jointMatrices(MaxJoints);
	m_animator.Update(m_headJoint, jointMatrices);
	m_storageAnimation.Push(jointMatrices.data(), sizeof(Matrix4) * jointMatrices.size());
}

bool MeshAnimated::CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline::Stage &pipelineStage) {
	if (!m_model || !m_material)
		return false;

	// Checks if the mesh is in view.
	/*if (auto rigidbody = GetEntity()->GetComponent<Rigidbody>()) {
		if (!rigidbody->InFrustum(Scenes::Get()->GetCamera()->GetViewFrustum()))
			return false;
	}*/

	// Check if we are in the correct pipeline stage.
	auto materialPipeline = m_material->GetPipelineMaterial();
	if (!materialPipeline || materialPipeline->GetStage() != pipelineStage)
		return false;

	// Binds the material pipeline.
	if (!materialPipeline->BindPipeline(commandBuffer))
		return false;

	const auto &pipeline = *materialPipeline->GetPipeline();

	// Updates descriptors.
	m_descriptorSet.Push("UniformScene", uniformScene);
	m_descriptorSet.Push("UniformObject", m_uniformObject);
	m_descriptorSet.Push("BufferAnimation", m_storageAnimation);

	m_material->PushDescriptors(m_descriptorSet);

	if (!m_descriptorSet.Update(pipeline))
		return false;

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return m_model->CmdRender(commandBuffer);
}

void MeshAnimated::SetMaterial(std::unique_ptr<Material> &&material) {
	m_material = std::move(material);
	m_material->Start(GetVertexInput(), true);
}

const Node &operator>>(const Node &node, MeshAnimated &meshAnimated) {
	node["filename"].Get(meshAnimated.m_filename);
	node["material"].Get(meshAnimated.m_material);
	return node;
}

Node &operator<<(Node &node, const MeshAnimated &meshAnimated) {
	node["filename"].Set(meshAnimated.m_filename);
	node["material"].Set(meshAnimated.m_material);
	return node;
}
}

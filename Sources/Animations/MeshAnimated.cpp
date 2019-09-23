#include "MeshAnimated.hpp"

#include "Maths/Maths.hpp"
#include "Files/File.hpp"
#include "Scenes/Entity.hpp"
#include "Files/Json/Json.hpp"
//#include "Files/Xml/Xml.hpp"
#include "Skeleton/SkeletonLoader.hpp"
#include "Skin/SkinLoader.hpp"

namespace acid {
bool MeshAnimated::registered = Register("meshAnimated");

MeshAnimated::MeshAnimated(std::filesystem::path filename) :
	m_filename(std::move(filename)) {
}

void MeshAnimated::Start() {
	if (m_filename.empty()) {
		return;
	}

	//File file(m_filename, std::make_unique<Xml>("COLLADA"));
	//file.Load();
	//auto &fileNode = *file.GetNode();
	File file(std::make_unique<Json>());
	file.Load(m_filename);
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
		File fileModel(std::make_unique<Json>());
		(*fileModel.GetNode())["vertices"].Set(m_model->GetVertices<VertexAnimated>());
		(*fileModel.GetNode())["indices"].Set(m_model->GetIndices());
		fileModel.Write("Animation/Model.json", Node::Format::Beautified);
	}
	{
		File fileJoints(std::make_unique<Json>());
		*fileJoints.GetNode() << m_headJoint;
		fileJoints.Write("Animation/Joints.json", Node::Format::Beautified);
	}
	{
		File fileAnimation0(std::make_unique<Json>());
		*fileAnimation0.GetNode() << *m_animation;
		fileAnimation0.Write("Animation/Animation0.json", Node::Format::Beautified);
	}
#endif*/
}

void MeshAnimated::Update() {
	std::vector<Matrix4> jointMatrices(MaxJoints);
	m_animator.Update(m_headJoint, jointMatrices);
	m_storageAnimation.Push(jointMatrices.data(), sizeof(Matrix4) * jointMatrices.size());
}

const Node &operator>>(const Node &node, MeshAnimated &meshAnimated) {
	node["filename"].Get(meshAnimated.m_filename);
	return node;
}

Node &operator<<(Node &node, const MeshAnimated &meshAnimated) {
	node["filename"].Set(meshAnimated.m_filename);
	return node;
}
}

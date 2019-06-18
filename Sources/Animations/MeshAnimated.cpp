#include "MeshAnimated.hpp"

#include "Maths/Maths.hpp"
#include "Files/File.hpp"
#include "Scenes/Entity.hpp"
#include "Serialized/Json/Json.hpp"
#include "Serialized/Xml/Xml.hpp"
#include "Skeleton/SkeletonLoader.hpp"
#include "Skin/SkinLoader.hpp"

namespace acid
{
MeshAnimated::MeshAnimated(std::filesystem::path filename) :
	m_filename{std::move(filename)}
{
}

void MeshAnimated::Start()
{
	if (m_filename.empty())
	{
		return;
	}

	File file{m_filename, std::make_unique<Xml>("COLLADA")};
	file.Load();

	// Because in Blender z is up, but Acid is y up. A correction must be applied to positions and normals.
	static const auto Correction{Matrix4{}.Rotate(Maths::Radians(-90.0f), Vector3f::Right)};

	SkinLoader skinLoader{file.GetMetadata()->FindChild("library_controllers"), MaxWeights};
	SkeletonLoader skeletonLoader{file.GetMetadata()->FindChild("library_visual_scenes"), skinLoader.GetJointOrder(), Correction};
	GeometryLoader geometryLoader{file.GetMetadata()->FindChild("library_geometries"), skinLoader.GetVertexWeights(), Correction};

	m_model = std::make_shared<Model>(geometryLoader.GetVertices(), geometryLoader.GetIndices());
	m_headJoint = skeletonLoader.GetHeadJoint();

	AnimationLoader animationLoader{file.GetMetadata()->FindChild("library_animations"), file.GetMetadata()->FindChild("library_visual_scenes"), Correction};

	m_animation = std::make_unique<Animation>(animationLoader.GetLengthSeconds(), animationLoader.GetKeyframes());
	m_animator.DoAnimation(m_animation.get());

	/*{
		File fileModel{"Animation/Model.json", std::make_unique<Json>()};
		fileModel.GetMetadata()->SetChild("vertices", m_model->GetVertices<VertexAnimated>());
		fileModel.GetMetadata()->SetChild("indices", m_model->GetIndices());
		fileModel.Write();
	}
	{
		File fileJoints{"Animation/Joints.json", std::make_unique<Json>()};
		*fileJoints.GetMetadata() << m_headJoint;
		fileJoints.Write();
	}
	{
		File fileAnimation0{"Animation/Animation0.json", std::make_unique<Json>()};
		*fileAnimation0.GetMetadata() << *m_animation;
		fileAnimation0.Write();
	}*/
}

void MeshAnimated::Update()
{
	std::vector<Matrix4> jointMatrices(MaxJoints);
	m_animator.Update(m_headJoint, jointMatrices);
	m_storageAnimation.Push(jointMatrices.data(), sizeof(Matrix4) * jointMatrices.size());
}

const Metadata &operator>>(const Metadata &metadata, MeshAnimated &meshAnimated)
{
	metadata.GetChild("filename", meshAnimated.m_filename);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const MeshAnimated &meshAnimated)
{
	metadata.SetChild("filename", meshAnimated.m_filename);
	return metadata;
}
}

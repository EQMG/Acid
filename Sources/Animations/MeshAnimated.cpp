#include "MeshAnimated.hpp"

#include "Maths/Maths.hpp"
#include "Files/File.hpp"
#include "Scenes/Entity.hpp"
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
	auto correction{Matrix4{}.Rotate(Maths::Radians(-90.0f), Vector3f::Right)};

	SkinLoader skinLoader{file.GetMetadata()->FindChild("library_controllers"), MaxWeights};
	SkeletonLoader skeletonLoader{file.GetMetadata()->FindChild("library_visual_scenes"), skinLoader.GetJointOrder(), correction};
	GeometryLoader geometryLoader{file.GetMetadata()->FindChild("library_geometries"), skinLoader.GetVertexWeights(), correction};

	m_model = std::make_shared<Model>(geometryLoader.GetVertices(), geometryLoader.GetIndices());
	m_headJoint = CreateJoints(*skeletonLoader.GetHeadJoint());
	m_headJoint->CalculateInverseBindTransform(Matrix4{});

	AnimationLoader animationLoader{file.GetMetadata()->FindChild("library_animations"), file.GetMetadata()->FindChild("library_visual_scenes"), correction};

	m_animation = std::make_unique<Animation>(animationLoader.GetLengthSeconds(), animationLoader.GetKeyframes());
	m_animator.DoAnimation(m_animation.get());
}

void MeshAnimated::Update()
{
	if (m_headJoint != nullptr)
	{
		std::vector<Matrix4> jointMatrices(MaxJoints);
		m_animator.Update(m_headJoint.get(), jointMatrices);
		m_storageAnimation.Push(jointMatrices.data(), sizeof(Matrix4) * jointMatrices.size());
	}
}

std::unique_ptr<Joint> MeshAnimated::CreateJoints(const JointData &data)
{
	auto joint{std::make_unique<Joint>(data.GetIndex(), data.GetNameId(), data.GetBindLocalTransform())};

	for (const auto &child : data.GetChildren())
	{
		joint->AddChild(CreateJoints(*child));
	}

	return joint;
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

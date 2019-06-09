#include "MeshAnimated.hpp"

#include "Maths/Maths.hpp"
#include "Files/File.hpp"
#include "Serialized/Xml/Xml.hpp"
#include "Skeleton/SkeletonLoader.hpp"
#include "Skin/SkinLoader.hpp"

namespace acid
{
MeshAnimated::MeshAnimated(std::string filename) :
	m_filename{std::move(filename)}
{
	Load();
}

void MeshAnimated::Update()
{
	if (m_animator != nullptr)
	{
		m_animator->Update();
	}

	if (m_headJoint != nullptr)
	{
		m_jointMatrices.clear();
		m_jointMatrices.resize(MaxJoints);
		AddJointsToArray(*m_headJoint, m_jointMatrices);
		//m_jointMatrices.shrink_to_fit();
	}
}

void MeshAnimated::Load()
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
	m_animator = std::make_unique<Animator>(m_headJoint.get());

	AnimationLoader animationLoader{file.GetMetadata()->FindChild("library_animations"), file.GetMetadata()->FindChild("library_visual_scenes"), correction};

	m_animation = std::make_unique<Animation>(animationLoader.GetLengthSeconds(), animationLoader.GetKeyframes());
	m_animator->DoAnimation(m_animation.get());
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

void MeshAnimated::AddJointsToArray(const Joint &headJoint, std::vector<Matrix4> &jointMatrices)
{
	if (headJoint.GetIndex() < jointMatrices.size())
	{
		jointMatrices[headJoint.GetIndex()] = headJoint.GetAnimatedTransform();
	}

	for (const auto &childJoint : headJoint.GetChildren())
	{
		AddJointsToArray(*childJoint, jointMatrices);
	}
}

const Metadata &operator>>(const Metadata &metadata, MeshAnimated &meshAnimated)
{
	metadata.GetChild("Model", meshAnimated.m_filename);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const MeshAnimated &meshAnimated)
{
	metadata.SetChild("Model", meshAnimated.m_filename);
	return metadata;
}
}

#include "MeshAnimated.hpp"

#include "Maths/Maths.hpp"
#include "Files/File.hpp"
#include "Serialized/Xml/Xml.hpp"
#include "Files/FileSystem.hpp"

namespace acid
{
	const Matrix4 MeshAnimated::Correction = Matrix4(Matrix4::Identity.Rotate(-90.0f * Maths::DegToRad, Vector3::Right));
	const uint32_t MeshAnimated::MaxJoints = 50;
	const uint32_t MeshAnimated::MaxWeights = 3;

	MeshAnimated::MeshAnimated(const std::string &filename) :
		Mesh(),
		m_filename(filename),
		m_model(nullptr),
		m_headJoint(nullptr),
		m_animator(nullptr),
		m_animation(nullptr),
		m_jointMatrices(std::vector<Matrix4>())
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
		//	m_jointMatrices.shrink_to_fit();
		}
	}

	void MeshAnimated::Load()
	{
		if (m_filename.empty())
		{
			return;
		}

		File file = File(m_filename, new Xml("COLLADA"));
		file.Read();

		SkinLoader skinLoader = SkinLoader(file.GetMetadata()->FindChild("library_controllers"), MaxWeights);
		SkeletonLoader skeletonLoader = SkeletonLoader(file.GetMetadata()->FindChild("library_visual_scenes"), skinLoader.GetJointOrder());
		GeometryLoader geometryLoader = GeometryLoader(file.GetMetadata()->FindChild("library_geometries"), skinLoader.GetVertexWeights());

		auto vertices = geometryLoader.GetVertices();
		auto indices = geometryLoader.GetIndices();
		m_model = std::make_shared<Model>(vertices, indices);
		m_headJoint.reset(CreateJoints(*skeletonLoader.GetHeadJoint()));
		m_headJoint->CalculateInverseBindTransform(Matrix4::Identity);
		m_animator = std::make_unique<Animator>(m_headJoint.get());

		AnimationLoader animationLoader = AnimationLoader(file.GetMetadata()->FindChild("library_animations"),
		                                                  file.GetMetadata()->FindChild("library_visual_scenes"));
		m_animation = std::make_unique<Animation>(animationLoader.GetLengthSeconds(), animationLoader.GetKeyframes());
		m_animator->DoAnimation(m_animation.get());
	}

	void MeshAnimated::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Model", m_filename);
	}

	void MeshAnimated::Encode(Metadata &metadata) const
	{
		metadata.SetChild("Model", m_filename);
	}

	Joint *MeshAnimated::CreateJoints(const JointData &data)
	{
		auto joint = new Joint(data.GetIndex(), data.GetNameId(), data.GetBindLocalTransform());

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
}

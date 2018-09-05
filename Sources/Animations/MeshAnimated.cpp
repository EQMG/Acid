#include "MeshAnimated.hpp"

#include "Files/Xml/FileXml.hpp"
#include "Helpers/FileSystem.hpp"

namespace acid
{
	const Matrix4 MeshAnimated::CORRECTION = Matrix4(Matrix4::IDENTITY.Rotate(Maths::Radians(-90.0f), Vector3::RIGHT));
	const uint32_t MeshAnimated::MAX_JOINTS = 50;
	const uint32_t MeshAnimated::MAX_WEIGHTS = 3;

	MeshAnimated::MeshAnimated(const std::string &filename) :
		Mesh(),
		m_filename(Files::SearchFile(filename)),
		m_model(nullptr),
		m_headJoint(nullptr),
		m_animator(nullptr),
		m_animation(nullptr),
		m_jointMatrices(std::vector<Matrix4>())
	{
		TrySetModel(m_filename);
	}

	MeshAnimated::~MeshAnimated()
	{
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
			m_jointMatrices.resize(MAX_JOINTS);
			AddJointsToArray(*m_headJoint, m_jointMatrices);
		//	m_jointMatrices.shrink_to_fit();
		}
	}

	void MeshAnimated::Decode(const Node &node)
	{
		TrySetModel(node.GetChild<std::string>("Model"));
	}

	void MeshAnimated::Encode(Node &node) const
	{
		node.SetChild<std::string>("Model", m_filename);
	}

	void MeshAnimated::TrySetModel(const std::string &filename)
	{
		if (!FileSystem::FileExists(filename))
		{
			Log::Error("Animation file does not exist: '%s'\n", m_filename.c_str());
			return;
		}

		FileXml file = FileXml(filename);
		file.Load();

		SkinLoader skinLoader = SkinLoader(file.GetParent()->FindChild("COLLADA")->FindChild("library_controllers"), MAX_WEIGHTS);
		SkeletonLoader skeletonLoader = SkeletonLoader(file.GetParent()->FindChild("COLLADA")->FindChild("library_visual_scenes"), skinLoader.GetJointOrder());
		GeometryLoader geometryLoader = GeometryLoader(file.GetParent()->FindChild("COLLADA")->FindChild("library_geometries"), skinLoader.GetVerticesSkinData());

		auto vertices = geometryLoader.GetVertices();
		auto indices = geometryLoader.GetIndices();
		m_model = std::make_shared<Model>(vertices, indices, filename);
		m_headJoint = CreateJoints(*skeletonLoader.GetHeadJoint());
		m_headJoint->CalculateInverseBindTransform(Matrix4::IDENTITY);
		m_animator = std::make_shared<Animator>(m_headJoint);

		AnimationLoader animationLoader = AnimationLoader(file.GetParent()->FindChild("COLLADA")->FindChild("library_animations"),
			file.GetParent()->FindChild("COLLADA")->FindChild("library_visual_scenes"));
		m_animation = std::make_shared<Animation>(animationLoader.GetLengthSeconds(), animationLoader.GetKeyframeData());
		m_animator->DoAnimation(m_animation);
	}

	std::shared_ptr<Joint> MeshAnimated::CreateJoints(const JointData &data)
	{
		auto joint = std::make_shared<Joint>(data.GetIndex(), data.GetNameId(), data.GetBindLocalTransform());

		for (auto &child : data.GetChildren())
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

		for (auto &childJoint : headJoint.GetChildren())
		{
			AddJointsToArray(*childJoint, jointMatrices);
		}
	}
}

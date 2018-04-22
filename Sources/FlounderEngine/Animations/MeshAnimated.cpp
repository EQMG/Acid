#include "MeshAnimated.hpp"

#include "Files/Json/FileJson.hpp"
#include "Helpers/FileSystem.hpp"

namespace Flounder
{
	const Matrix4 *MeshAnimated::S_CORRECTION = new Matrix4(Matrix4::IDENTITY.Rotate(Maths::Radians(-90.0f), Vector3::RIGHT));
	const int MeshAnimated::MAX_WEIGHTS = 3;

	MeshAnimated::MeshAnimated(const std::string &filename) :
		Mesh(),
		m_filename(filename),
		m_model(nullptr),
		m_headJoint(nullptr),
		m_animation(nullptr),
		m_animator(nullptr),
		m_jointMatrices(std::vector<Matrix4 *>())
	{
	}

	MeshAnimated::~MeshAnimated()
	{
		delete m_headJoint;
		delete m_model;
		delete m_animation;
		delete m_animator;
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
			AddJointsToArray(*m_headJoint, &m_jointMatrices);
		}
	}

	void MeshAnimated::Load(LoadedValue *value)
	{
		TrySetModel(value->GetChild("Model")->GetString());
	}

	void MeshAnimated::Write(LoadedValue *value)
	{
		value->GetChild("Model", true)->SetString(m_filename);
	}

	void MeshAnimated::TrySetModel(const std::string &filename)
	{
		delete m_animation;
		delete m_headJoint;
		delete m_model;
		delete m_animator;

		if (!FileSystem::FileExists(filename))
		{
			fprintf(stderr, "Animation file does not exist: '%s'\n", m_filename.c_str());
			return;
		}

		FileJson *file = new FileJson(filename);
		file->Load();

		SkinLoader *skinLoader = new SkinLoader(file->GetParent()->GetChild("COLLADA")->GetChild("library_controllers"), MAX_WEIGHTS);
		SkeletonLoader *skeletonLoader = new SkeletonLoader(file->GetParent()->GetChild("COLLADA")->GetChild("library_visual_scenes"),
			skinLoader->GetJointOrder());
		GeometryLoader *geometryLoader = new GeometryLoader(file->GetParent()->GetChild("COLLADA")->GetChild("library_geometries"), skinLoader->GetVerticesSkinData());
		auto vertices = geometryLoader->GetVertices();
		auto indices = geometryLoader->GetIndices();
		m_model = new Model(vertices, indices);
		m_headJoint = CreateJoints(skeletonLoader->GetHeadJoint());
		//	delete skinLoader;
		//	delete skeletonLoader;
		//	delete geometryLoader;

		AnimationLoader *animationLoader = new AnimationLoader(file->GetParent()->GetChild("COLLADA")->GetChild("library_animations"),
			file->GetParent()->GetChild("COLLADA")->GetChild("library_visual_scenes"));
		m_animation = new Animation(animationLoader->GetLengthSeconds(), animationLoader->GetKeyframeData());
		//	delete animationLoader;

		delete file;

		m_animator = new Animator(m_headJoint);
		m_animator->DoAnimation(m_animation);
	}

	Joint *MeshAnimated::CreateJoints(JointData *data)
	{
		Joint *j = new Joint(data->GetIndex(), data->GetNameId(), data->GetBindLocalTransform());

		for (auto child : data->GetChildren())
		{
			j->AddChild(CreateJoints(child));
		}

		return j;
	}

	void MeshAnimated::AddJointsToArray(const Joint &headJoint, std::vector<Matrix4 *> *jointMatrices)
	{
		if (headJoint.GetIndex() < (int) jointMatrices->size())
		{
			jointMatrices->at(headJoint.GetIndex()) = headJoint.GetAnimatedTransform();
		}

		for (auto childJoint : *headJoint.GetChildren())
		{
			AddJointsToArray(*childJoint, jointMatrices);
		}
	}
}

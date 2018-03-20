#include "MeshAnimated.hpp"

#include "../../Files/Json/FileJson.hpp"
#include "../../Helpers/FileSystem.hpp"

namespace Flounder
{
	const Matrix4 *MeshAnimated::S_CORRECTION = Matrix4::Rotate(Matrix4(), Vector3::RIGHT, Maths::Radians(-90.0f), nullptr);
	const int MeshAnimated::MAX_WEIGHTS = 3;

	MeshAnimated::MeshAnimated(const std::string &filename) :
		Component(),
		m_filename(filename),
		m_model(nullptr),
		m_headJoint(nullptr),
		m_animation(nullptr),
		m_animator(nullptr)
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
		m_animator->Update();
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
			skinLoader->GetData()->GetJointOrder());
		GeometryLoader *geometryLoader = new GeometryLoader(file->GetParent()->GetChild("COLLADA")->GetChild("library_geometries"), skinLoader->GetData()->GetVerticesSkinData());
	//	m_model = new Model(geometryLoader->GetData()->GetVertices(), geometryLoader->GetData()->GetIndices());
		m_headJoint = CreateJoints(skeletonLoader->GetData()->GetHeadJoint());
		delete skinLoader;
		delete skeletonLoader;
		delete geometryLoader;

		AnimationLoader *animationLoader = new AnimationLoader(file->GetParent()->GetChild("COLLADA")->GetChild("library_animations"),
			file->GetParent()->GetChild("COLLADA")->GetChild("library_visual_scenes"));
		m_animation = new Animation(*animationLoader->GetData());
		delete animationLoader;

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
}

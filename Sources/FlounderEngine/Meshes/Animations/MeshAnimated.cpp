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
		m_animation(nullptr)
	{
	}

	MeshAnimated::~MeshAnimated()
	{
		delete m_animation;
		delete m_model;
	}

	void MeshAnimated::Update()
	{
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
		delete m_model;

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
		delete skinLoader;
		delete skeletonLoader;
		delete geometryLoader;

		AnimationLoader *animationLoader = new AnimationLoader(file->GetParent()->GetChild("COLLADA")->GetChild("library_animations"),
			file->GetParent()->GetChild("COLLADA")->GetChild("library_visual_scenes"));
		m_animation = new Animation(*animationLoader->GetData());
		delete animationLoader;

		delete file;
	}
}

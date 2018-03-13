#include "MeshAnimated.hpp"

#include "../Files/Json/FileJson.hpp"
#include "Skeleton/Skeleton.hpp"

namespace Flounder
{
	const Matrix4 *MeshAnimated::S_CORRECTION = Matrix4::Rotate(Matrix4(), Vector3::RIGHT, Maths::Radians(-90.0f), nullptr);
	const int MeshAnimated::MAX_WEIGHTS = 3;


	MeshAnimated::MeshAnimated(const std::string &filename) :
		m_filename(filename)
	{
		FileJson *file = new FileJson(filename);
		file->Load();

		//Skin *skin = new Skin(file->GetParent()->GetChild("COLLADA")->GetChild("library_controllers"));
		//Skeleton *skeleton = new Skeleton(file->GetParent()->GetChild("COLLADA")->GetChild("library_visual_scenes"), skin->GetData()->GetJointOrder());
		//GeometryLoader *geometry = new Geometry(file->GetParent()->GetChild("COLLADA")->GetChild("library_geometries"), skin->GetData()->GetVerticesSkinData());
		//Animation *animation = new Animation(file->GetParent()->GetChild("COLLADA")->GetChild("library_animations"),
		// 	file->GetParent()->GetChild("COLLADA")->GetChild("library_visual_scenes"));

		delete file;
	}

	MeshAnimated::~MeshAnimated()
	{

	}
}

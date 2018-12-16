#include "ModelRegister.hpp"

#include "Helpers/FileSystem.hpp"
#include "Obj/ModelObj.hpp"
#include "Shapes/ModelCube.hpp"
#include "Shapes/ModelCylinder.hpp"
#include "Shapes/ModelDisk.hpp"
#include "Shapes/ModelRectangle.hpp"
#include "Shapes/ModelSphere.hpp"

namespace acid
{
	ModelRegister::ModelRegister() :
		m_models(std::map<std::string, ModelCreate>())
	{
		Add<ModelObj>(".obj");
		Add<ModelCube>("Cube_");
		Add<ModelCylinder>("Cylinder_");
		Add<ModelDisk>("Disk_");
		Add<ModelRectangle>("Rectangle_");
		Add<ModelSphere>("Sphere_");
	}

	void ModelRegister::Remove(const std::string &name)
	{
		//	m_models.erase(std::remove_if(m_models.begin(), m_models.end(), [name](const std::string &n, const ComponentCreate &c){
		//		return name == n; // FIXME: Remove
		//	}), m_models.end());
	}

	std::shared_ptr<Model> ModelRegister::Create(const std::string &data)
	{
		auto fileExt = String::Lowercase(FileSystem::FileSuffix(data));
		auto split = String::Split(data, "_");

		auto it = m_models.find(fileExt);

		if (it == m_models.end())
		{
			if (!split.empty())
			{
				it = m_models.find(split[0] + "_");
			}

			if (it == m_models.end())
			{
				Log::Error("Could not find registered model from data: '%s'\n", data.c_str());
				return nullptr;
			}
		}

		return ((*it).second)(data);
	}
}

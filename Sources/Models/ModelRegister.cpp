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
		m_mutex(std::mutex()),
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
		std::lock_guard<std::mutex> lock(m_mutex);

		m_models.erase(name);
	}

	std::shared_ptr<Model> ModelRegister::Create(const std::string &data) const
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

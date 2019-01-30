#include "ModelRegister.hpp"

#include "Files/FileSystem.hpp"
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
		Add<ModelObj>("ModelObj");
		Add<ModelCube>("ModelCube");
		Add<ModelCylinder>("ModelCylinder");
		Add<ModelDisk>("ModelDisk");
		Add<ModelRectangle>("ModelRectangle");
		Add<ModelSphere>("ModelSphere");
	}

	void ModelRegister::Remove(const std::string &name)
	{
		m_models.erase(name);
	}

	std::shared_ptr<Model> ModelRegister::Create(const Metadata &metadata) const
	{
		auto typeName = metadata.GetChild<std::string>("Type");

		auto it = m_models.find(typeName);

		if (it == m_models.end())
		{
			Log::Error("Could not find registered model by name: '%s'\n", typeName.c_str());
			return nullptr;
		}

		return ((*it).second)(metadata);
	}
}

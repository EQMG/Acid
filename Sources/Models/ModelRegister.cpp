#include "ModelRegister.hpp"

#include "Gltf/ModelGltf.hpp"
#include "Obj/ModelObj.hpp"
#include "Shapes/ModelCube.hpp"
#include "Shapes/ModelCylinder.hpp"
#include "Shapes/ModelDisk.hpp"
#include "Shapes/ModelRectangle.hpp"
#include "Shapes/ModelSphere.hpp"

namespace acid
{
ModelRegister::ModelRegister()
{
	AddNode<ModelGltf>("gltf");
	AddExtension<ModelGltf>(".gltf");
	AddExtension<ModelGltf>(".glb");
	AddNode<ModelObj>("obj");
	AddExtension<ModelObj>(".obj");
	AddNode<ModelCube>("cube");
	AddNode<ModelCylinder>("cylinder");
	AddNode<ModelDisk>("disk");
	AddNode<ModelRectangle>("rectangle");
	AddNode<ModelSphere>("sphere");
}

void ModelRegister::Remove(const std::string &name)
{
	m_modelNodes.erase(name);
	m_modelExtensions.erase(name);
}

std::shared_ptr<Model> ModelRegister::Create(const Node &node) const
{
	auto typeName{node["type"].Get<std::string>()};

	auto it{m_modelNodes.find(typeName)};

	if (it == m_modelNodes.end())
	{
		Log::Warning("Could not find registered model by name: '%s'\n", typeName);
		return nullptr;
	}

	return (*it).second(node);
}

std::shared_ptr<Model> ModelRegister::Create(const std::filesystem::path &filename) const
{
	auto fileExt{filename.extension().string()};
	auto it{m_modelExtensions.find(fileExt)};

	if (it == m_modelExtensions.end())
	{
		Log::Warning("Could not find registered model by extension: '%s'\n", fileExt);
		return nullptr;
	}

	return (*it).second(filename);
}
}

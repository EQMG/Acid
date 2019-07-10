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
	AddMetadata<ModelGltf>("gltf");
	AddExtension<ModelGltf>(".gltf");
	AddExtension<ModelGltf>(".glb");
	AddMetadata<ModelObj>("obj");
	AddExtension<ModelObj>(".obj");
	AddMetadata<ModelCube>("cube");
	AddMetadata<ModelCylinder>("cylinder");
	AddMetadata<ModelDisk>("disk");
	AddMetadata<ModelRectangle>("rectangle");
	AddMetadata<ModelSphere>("sphere");
}

void ModelRegister::Remove(const std::string &name)
{
	m_modelMetadatas.erase(name);
	m_modelExtensions.erase(name);
}

std::shared_ptr<Model> ModelRegister::Create(const Metadata &metadata) const
{
	auto typeName{metadata.GetChild<std::string>("type")};

	auto it{m_modelMetadatas.find(typeName)};

	if (it == m_modelMetadatas.end())
	{
		std::cerr << "Could not find registered model by name: " << std::quoted(typeName) << '\n';
		return nullptr;
	}

	return (*it).second(metadata);
}

std::shared_ptr<Model> ModelRegister::Create(const std::filesystem::path &filename) const
{
	auto fileExt{filename.extension().string()};
	auto it{m_modelExtensions.find(fileExt)};

	if (it == m_modelExtensions.end())
	{
		std::cerr << "Could not find registered model by extension: " << std::quoted(fileExt) << '\n';
		return nullptr;
	}

	return (*it).second(filename);
}
}

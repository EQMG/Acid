#include "ModelRegister.hpp"

#include "Files/FileSystem.hpp"
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
	AddMetadata<ModelGltf>("ModelGltf");
	AddExtension<ModelGltf>(".gltf");
	AddExtension<ModelGltf>(".glb");
	AddMetadata<ModelObj>("ModelObj");
	AddExtension<ModelObj>(".obj");
	AddMetadata<ModelCube>("ModelCube");
	AddMetadata<ModelCylinder>("ModelCylinder");
	AddMetadata<ModelDisk>("ModelDisk");
	AddMetadata<ModelRectangle>("ModelRectangle");
	AddMetadata<ModelSphere>("ModelSphere");
}

void ModelRegister::Remove(const std::string& name)
{
	m_modelMetadatas.erase(name);
	m_modelExtensions.erase(name);
}

std::shared_ptr<Model> ModelRegister::Create(const Metadata& metadata) const
{
	auto typeName = metadata.GetChild<std::string>("Type");

	auto it = m_modelMetadatas.find(typeName);

	if(it == m_modelMetadatas.end())
		{
			Log::Error("Could not find registered model by name: '%s'\n", typeName.c_str());
			return nullptr;
		}

	return ((*it).second)(metadata);
}

std::shared_ptr<Model> ModelRegister::Create(const std::string& filename) const
{
	auto fileExt = String::Lowercase(FileSystem::FileSuffix(filename));
	auto it = m_modelExtensions.find(fileExt);

	if(it == m_modelExtensions.end())
		{
			Log::Error("Could not find registered model by extension: '%s'\n", fileExt.c_str());
			return nullptr;
		}

	return ((*it).second)(filename);
}
}

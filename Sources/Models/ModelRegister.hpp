#pragma once

#include "Engine/Log.hpp"
#include "Serialized/Metadata.hpp"
#include "Model.hpp"

namespace acid
{
/// <summary>
/// A class that holds registered model types.
/// </summary>
class ACID_EXPORT ModelRegister
{
public:
	/// <summary>
	/// Creates a new model types register.
	/// </summary>
	ModelRegister();

	/// <summary>
	/// Adds a model type to this register, model created by Metadata object.
	/// </summary>
	/// <param name="name"> The models type name. </param>
	/// <param name="T"> The models type. </param>
	template<typename T> void AddMetadata(const std::string &name)
	{
		if (m_modelMetadatas.find(name) != m_modelMetadatas.end())
		{
			Log::Error("Model metadata type '%s' is already registered!\n", name.c_str());
			return;
		}

		auto modelCreate = [](const Metadata &metadata) -> std::shared_ptr<Model>
		{
			return T::Create(metadata);
		};
		m_modelMetadatas.emplace(name, modelCreate);
	}

	/// <summary>
	/// Adds a model type to this register, model created by filename.
	/// </summary>
	/// <param name="extension"> The file extension this can be loaded from. </param>
	/// <param name="T"> The models type. </param>
	template<typename T> void AddExtension(const std::string &extension)
	{
		if (m_modelMetadatas.find(extension) != m_modelMetadatas.end())
		{
			Log::Error("Model extension type '%s' is already registered!\n", extension.c_str());
			return;
		}

		auto modelCreate = [](const std::string &filename) -> std::shared_ptr<Model>
		{
			return T::Create(filename);
		};
		m_modelExtensions.emplace(extension, modelCreate);
	}

	/// <summary>
	/// Removes a model type from the metadata or extensions register.
	/// </summary>
	/// <param name="name"> The model types name. </param>
	void Remove(const std::string &name);

	/// <summary>
	/// Creates a new model from the register.
	/// </summary>
	/// <param name="metadata"> The metadata to decode values from. </param>
	/// <returns> The new model. </returns>
	std::shared_ptr<Model> Create(const Metadata &metadata) const;

	/// <summary>
	/// Creates a new model from the register.
	/// </summary>
	/// <param name="filename"> The models filename to load from. </param>
	/// <returns> The new model. </returns>
	std::shared_ptr<Model> Create(const std::string &filename) const;

private:
	using ModelMetadataCreate = std::function<std::shared_ptr<Model>(const Metadata &)>;
	using ModelFilenameCreate = std::function<std::shared_ptr<Model>(const std::string &)>;
	std::map<std::string, ModelMetadataCreate> m_modelMetadatas;
	std::map<std::string, ModelFilenameCreate> m_modelExtensions;
};
}

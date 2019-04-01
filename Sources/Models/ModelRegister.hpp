#pragma once

#include "Engine/Log.hpp"
#include "Serialized/Metadata.hpp"
#include "Model.hpp"

namespace acid
{
/**
 * @brief Class that holds registered model types.
 */
class ACID_EXPORT ModelRegister
{
public:
	/**
	 * Creates a new model types register.
	 */
	ModelRegister();

	/**
	 * Adds a model type to this register, model created by Metadata object.
	 * @tparam T The models type.
	 * @param name The models type name.
	 */
	template<typename T>
	void AddMetadata(const std::string &name)
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

	/**
	 * Adds a model type to this register, model created by filename.
	 * @tparam T The models type.
	 * @param extension The file extension this can be loaded from.
	 */
	template<typename T>
	void AddExtension(const std::string &extension)
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

	/**
	 * Removes a model type from the metadata or extensions register.
	 * @param name The model types name.
	 */
	void Remove(const std::string &name);

	/**
	 * Creates a new model from the register.
	 * @param metadata The metadata to decode values from.
	 * @return The new model.
	 */
	std::shared_ptr<Model> Create(const Metadata &metadata) const;

	/**
	 * Creates a new model from the register.
	 * @param filename The models filename to load from.
	 * @return The new model.
	 */
	std::shared_ptr<Model> Create(const std::string &filename) const;

private:
	using ModelMetadataCreate = std::function<std::shared_ptr<Model>(const Metadata &)>;
	using ModelFilenameCreate = std::function<std::shared_ptr<Model>(const std::string &)>;
	std::map<std::string, ModelMetadataCreate> m_modelMetadatas;
	std::map<std::string, ModelFilenameCreate> m_modelExtensions;
};
}

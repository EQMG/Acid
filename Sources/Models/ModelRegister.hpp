#pragma once

#include "Serialized/Node.hpp"
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
	 * Adds a model type to this register, model created by Node object.
	 * @tparam T The models type.
	 * @param name The models type name.
	 */
	template<typename T>
	void AddNode(const std::string &name)
	{
		if (m_modelNodes.find(name) != m_modelNodes.end())
		{
			std::cerr << "Model node type " << std::quoted(name) << " is already registered!\n";
			return;
		}

		auto modelCreate = [](const Node &node) -> std::shared_ptr<Model>
		{
			return T::Create(node);
		};
		m_modelNodes.emplace(name, modelCreate);
	}

	/**
	 * Adds a model type to this register, model created by filename.
	 * @tparam T The models type.
	 * @param extension The file extension this can be loaded from.
	 */
	template<typename T>
	void AddExtension(const std::string &extension)
	{
		if (m_modelNodes.find(extension) != m_modelNodes.end())
		{
			std::cerr << "Model extension type " << std::quoted(extension) << " is already registered!\n";
			return;
		}

		auto modelCreate = [](const std::filesystem::path &filename) -> std::shared_ptr<Model>
		{
			return T::Create(filename);
		};
		m_modelExtensions.emplace(extension, modelCreate);
	}

	/**
	 * Removes a model type from the node or extensions register.
	 * @param name The model types name.
	 */
	void Remove(const std::string &name);

	/**
	 * Creates a new model from the register.
	 * @param node The node to decode values from.
	 * @return The new model.
	 */
	std::shared_ptr<Model> Create(const Node &node) const;

	/**
	 * Creates a new model from the register.
	 * @param filename The models filename to load from.
	 * @return The new model.
	 */
	std::shared_ptr<Model> Create(const std::filesystem::path &filename) const;

private:
	using ModelNodeCreate = std::function<std::shared_ptr<Model>(const Node &)>;
	using ModelFilenameCreate = std::function<std::shared_ptr<Model>(const std::filesystem::path &)>;
	std::map<std::string, ModelNodeCreate> m_modelNodes;
	std::map<std::string, ModelFilenameCreate> m_modelExtensions;
};
}

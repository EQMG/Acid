#pragma once

#include "Models/Model.hpp"
#include "Graphics/Images/Image2d.hpp"

namespace acid {
/**
 * @brief Resource that represents a GLTF model.
 */
class ACID_EXPORT ModelGltf : public Model::Registrar<ModelGltf> {
public:
	/**
	 * Creates a new GLTF model, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The GLTF model with the requested values.
	 */
	static std::shared_ptr<ModelGltf> Create(const Node &node);

	/**
	 * Creates a new GLTF model, or finds one with the same values.
	 * @param filename The file to load the GLTF model from.
	 * @return The GLTF model with the requested values.
	 */
	static std::shared_ptr<ModelGltf> Create(const std::filesystem::path &filename);

	/**
	 * Creates a new GLTF model.
	 * @param filename The file to load the GLTF model from.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelGltf#Load} can be called later.
	 */
	explicit ModelGltf(std::filesystem::path filename, bool load = true);

	friend const Node &operator>>(const Node &node, ModelGltf &model);
	friend Node &operator<<(Node &node, const ModelGltf &model);

private:
	void Load();
	
	static bool registered;

	//struct Node;
	//struct Skin;

	std::filesystem::path m_filename;

	//std::vector<Node *> nodes;
	//std::vector<Node *> linearNodes;
	//std::vector<Skin *> m_skins;
};
}

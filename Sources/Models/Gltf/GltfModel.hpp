#pragma once

#include "Models/Model.hpp"
#include "Graphics/Images/Image2d.hpp"

namespace acid {
/**
 * @brief Resource that represents a GLTF model.
 */
class ACID_EXPORT GltfModel : public Model::Registrar<GltfModel> {
	inline static const bool Registered = Register("gltf", ".gltf");
public:
	/**
	 * Creates a new GLTF model, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The GLTF model with the requested values.
	 */
	static std::shared_ptr<GltfModel> Create(const Node &node);

	/**
	 * Creates a new GLTF model, or finds one with the same values.
	 * @param filename The file to load the GLTF model from.
	 * @return The GLTF model with the requested values.
	 */
	static std::shared_ptr<GltfModel> Create(const std::filesystem::path &filename);

	/**
	 * Creates a new GLTF model.
	 * @param filename The file to load the GLTF model from.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelGltf#Load} can be called later.
	 */
	explicit GltfModel(std::filesystem::path filename, bool load = true);

	friend const Node &operator>>(const Node &node, GltfModel &model);
	friend Node &operator<<(Node &node, const GltfModel &model);

private:
	void Load();
	
	//struct Node;
	//struct Skin;

	std::filesystem::path filename;

	//std::vector<Node *> nodes;
	//std::vector<Node *> linearNodes;
	//std::vector<Skin *> skins;
};
}

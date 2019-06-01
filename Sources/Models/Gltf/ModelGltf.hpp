#pragma once

#include "Models/Model.hpp"
#include "Graphics/Images/Image2d.hpp"

namespace acid
{
/**
 * @brief Resource that represents a GLTF model.
 */
class ACID_EXPORT ModelGltf :
	public Model
{
public:
	/**
	 * Creates a new GLTF model, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The GLTF model with the requested values.
	 */
	static std::shared_ptr<ModelGltf> Create(const Metadata &metadata);

	/**
	 * Creates a new GLTF model, or finds one with the same values.
	 * @param filename The file to load the GLTF model from.
	 * @return The GLTF model with the requested values.
	 */
	static std::shared_ptr<ModelGltf> Create(const std::string &filename);

	/**
	 * Creates a new GLTF model.
	 * @param filename The file to load the GLTF model from.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelGltf#Load} can be called later.
	 */
	explicit ModelGltf(std::string filename, const bool &load = true);

	void Load() override;

	friend const Metadata &operator>>(const Metadata &metadata, ModelGltf &model);

	friend Metadata &operator<<(Metadata &metadata, const ModelGltf &model);

private:
	//struct Node;
	//struct Skin;

	std::string m_filename;

	//std::vector<Node *> nodes;
	//std::vector<Node *> linearNodes;
	//std::vector<Skin *> m_skins;
};
}

#pragma once

#include "Models/Model.hpp"

namespace acid {
/**
 * @brief Resource that represents a OBJ model.
 */
class ACID_EXPORT ObjModel : public Model::Registrar<ObjModel> {
	inline static const bool Registered = Register("obj", ".obj");;
public:
	/**
	 * Creates a new OBJ model, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The OBJ model with the requested values.
	 */
	static std::shared_ptr<ObjModel> Create(const Node &node);

	/**
	 * Creates a new OBJ model, or finds one with the same values.
	 * @param filename The file to load the OBJ model from.
	 * @return The OBJ model with the requested values.
	 */
	static std::shared_ptr<ObjModel> Create(const std::filesystem::path &filename);

	/**
	 * Creates a new OBJ model.
	 * @param filename The file to load the OBJ model from.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelObj#Load} can be called later.
	 */
	explicit ObjModel(std::filesystem::path filename, bool load = true);

	friend const Node &operator>>(const Node &node, ObjModel &model);
	friend Node &operator<<(Node &node, const ObjModel &model);

private:
	void Load();
	
	std::filesystem::path filename;
};
}

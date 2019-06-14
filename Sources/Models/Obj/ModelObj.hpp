#pragma once

#include "Models/Model.hpp"

namespace acid
{
/**
 * @brief Resource that represents a OBL model.
 */
class ACID_EXPORT ModelObj :
	public Model
{
public:
	/**
	 * Creates a new OBJ model, or finds one with the same values.
	 * @param metadata The metadata to decode values from.
	 * @return The OBJ model with the requested values.
	 */
	static std::shared_ptr<ModelObj> Create(const Metadata &metadata);

	/**
	 * Creates a new OBJ model, or finds one with the same values.
	 * @param filename The file to load the OBJ model from.
	 * @return The OBJ model with the requested values.
	 */
	static std::shared_ptr<ModelObj> Create(const std::filesystem::path &filename);

	/**
	 * Creates a new OBJ model.
	 * @param filename The file to load the OBJ model from.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelObj#Load} can be called later.
	 */
	explicit ModelObj(std::filesystem::path filename, const bool &load = true);

	friend const Metadata &operator>>(const Metadata &metadata, ModelObj &model);

	friend Metadata &operator<<(Metadata &metadata, const ModelObj &model);

private:
	void Load();

	std::filesystem::path m_filename;
};
}

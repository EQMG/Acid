#pragma once

#include "Models/Model.hpp"

namespace acid
{
/**
 * @brief Resource that represents a OBL model.
 */
class ModelObj :
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
	static std::shared_ptr<ModelObj> Create(const std::string &filename);

	/**
	 * Creates a new OBJ model.
	 * @param filename The file to load the OBJ model from.
	 * @param load If this resource will be loaded immediately, otherwise {@link ModelObj#Load} can be called later.
	 */
	explicit ModelObj(std::string filename, const bool &load = true);

	void Load() override;

	friend const Metadata &operator>>(const Metadata &metadata, ModelObj &model);

	friend Metadata &operator<<(Metadata &metadata, const ModelObj &model);

private:
	std::string m_filename;
};
}

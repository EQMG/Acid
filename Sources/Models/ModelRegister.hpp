#pragma once

#include <functional>
#include <memory>
#include <optional>
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
		/// Adds a model types to this register, this model type must have a static Resource function that takes a data string.
		/// </summary>
		/// <param name="name"> The models type name. </param>
		/// <param name="T"> The models type. </param>
		template<typename T>
		void Add(const std::string &name)
		{
			if (m_models.find(name) != m_models.end())
			{
				Log::Error("Model type '%s' is already registered!\n", name.c_str());
				return;
			}

			auto modelCreate = [](const Metadata &metadata) -> std::shared_ptr<Model> {
				return T::Create(metadata);
			};

			m_models.emplace(name, modelCreate);
		}

		/// <summary>
		/// Removes a model type from the register.
		/// </summary>
		/// <param name="name"> The model types name. </param>
		void Remove(const std::string &name);

		/// <summary>
		/// Creates a new model from the register.
		/// </summary>
		/// <param name="data"> The models filename/data to create from. </param>
		/// <returns> The new model. </returns>
		std::shared_ptr<Model> Create(const Metadata &metadata) const;
	private:
		using ModelCreate = std::function<std::shared_ptr<Model>(const Metadata &)>;
		std::map<std::string, ModelCreate> m_models;
	};
}

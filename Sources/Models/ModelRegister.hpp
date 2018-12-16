#pragma once

#include <functional>
#include <memory>
#include <optional>
#include "Engine/Log.hpp"
#include "Model.hpp"

namespace acid
{
	/// <summary>
	/// A class that holds registered model types.
	/// </summary>
	class ACID_EXPORT ModelRegister
	{
	private:
		using ModelCreate = std::function<std::shared_ptr<Model>(std::string)>;
		std::map<std::string, ModelCreate> m_models;
	public:
		/// <summary>
		/// Creates a new model types register.
		/// </summary>
		ModelRegister();

		/// <summary>
		/// Adds a model types to this register, this model type must have a static Resource function that takes a data string.
		/// </summary>
		/// <param name="type"> The models type search params (file extension or prefix). </param>
		/// <param name="T"> The models type. </param>
		template<typename T>
		void Add(const std::string &type)
		{
			if (m_models.find(type) != m_models.end())
			{
				Log::Error("Model type '%s' is already registered!\n", type.c_str());
				return;
			}

			auto modelCreate = [](std::string data) -> std::shared_ptr<Model>
			{
				return T::Resource(data);
			};

			m_models.emplace(type, modelCreate);
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
		std::shared_ptr<Model> Create(const std::string &data);
	};
}

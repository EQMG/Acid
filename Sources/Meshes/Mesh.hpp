#pragma once

#include <string>
#include <vector>
#include "Objects/IComponent.hpp"
#include "Models/Model.hpp"

namespace fl
{
	class FL_EXPORT Mesh :
		public IComponent
	{
	private:
		std::shared_ptr<Model> m_model;

	public:
		Mesh(std::shared_ptr<Model> model = nullptr);

		~Mesh();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Mesh"; };

		virtual std::shared_ptr<Model> GetModel() const { return m_model; }

		virtual void SetModel(std::shared_ptr<Model> model) { m_model = model; }

		virtual void TrySetModel(const std::string &filename);
	};
}

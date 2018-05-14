#pragma once

#include <string>
#include <vector>
#include "Objects/Component.hpp"
#include "Models/Model.hpp"

namespace fl
{
	class FL_EXPORT Mesh :
		public Component
	{
	private:
		Model *m_model;

	public:
		Mesh(Model *model = nullptr);

		~Mesh();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "Mesh"; };

		virtual Model *GetModel() const { return m_model; }

		virtual void SetModel(Model *model) { m_model = model; }

		virtual void TrySetModel(const std::string &filename);
	};
}

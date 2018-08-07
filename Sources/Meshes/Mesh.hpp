#pragma once

#include <string>
#include <vector>
#include "Models/Model.hpp"
#include "Models/VertexModel.hpp"
#include "Objects/IComponent.hpp"

namespace acid
{
	class ACID_EXPORT Mesh :
		public IComponent
	{
	private:
		std::shared_ptr<Model> m_model;

	public:
		Mesh(std::shared_ptr<Model> model = nullptr);

		~Mesh();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		virtual std::shared_ptr<Model> GetModel() const { return m_model; }

		virtual VertexInput GetVertexInput() const { return VertexModel::GetVertexInput(); }

		virtual void SetModel(std::shared_ptr<Model> model) { m_model = model; }

		virtual void TrySetModel(const std::string &filename);
	};
}

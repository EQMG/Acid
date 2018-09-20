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
		explicit Mesh(const std::shared_ptr<Model> &model = nullptr);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		virtual std::shared_ptr<Model> GetModel() const { return m_model; }

		virtual VertexInput GetVertexInput(const uint32_t &binding = 0) const { return VertexModel::GetVertexInput(binding); }

		virtual void SetModel(const std::shared_ptr<Model> &model) { m_model = model; }

		virtual void TrySetModel(const std::string &filename);
	};
}

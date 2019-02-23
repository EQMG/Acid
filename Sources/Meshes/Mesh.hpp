#pragma once

#include "Models/Model.hpp"
#include "Models/VertexModel.hpp"
#include "Scenes/Component.hpp"

namespace acid
{
	/// <summary>
	/// A component that represents a model/mesh.
	/// </summary>
	class ACID_EXPORT Mesh :
		public Component
	{
	public:
		/// <summary>
		/// Creates a new mesh component with a model.
		/// </summary>
		/// <param name="model"> The initial model to use in this mesh. </param>
		explicit Mesh(const std::shared_ptr<Model> &model = nullptr);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		virtual const std::shared_ptr<Model> &GetModel() const { return m_model; }

		virtual Shader::VertexInput GetVertexInput(const uint32_t &binding = 0) const { return VertexModel::GetVertexInput(binding); }

		virtual void SetModel(const std::shared_ptr<Model> &model) { m_model = model; }
	private:
		std::shared_ptr<Model> m_model;
	};
}

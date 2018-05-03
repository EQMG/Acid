#pragma once

#include <vector>
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Renderer/Pipelines/PipelineCreate.hpp"
#include "IVertex.hpp"

namespace fl
{
	class FL_EXPORT VertexModel :
		public IVertex
	{
	public:
		Vector3 m_position;
		Vector2 m_uv;
		Vector3 m_normal;
		Vector3 m_tangent;

		VertexModel(const Vector3 &position = Vector3(), const Vector2 &uv = Vector2(), const Vector3 &normal = Vector3(), const Vector3 &tangent = Vector3());

		VertexModel(const VertexModel &source);

		~VertexModel();

		Vector3 GetPosition() const override { return m_position; };

		void SetPosition(const Vector3 &position) override { m_position = position; };

		size_t GetSize() const override { return sizeof(VertexModel); }

		void *GetData(std::vector<IVertex *> &vertices) override;

		static VertexInput GetVertexInput();
	};
}

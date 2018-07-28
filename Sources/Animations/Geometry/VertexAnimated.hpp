#pragma once

#include <vector>
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Models/IVertex.hpp"
#include "Renderer/Pipelines/PipelineCreate.hpp"

namespace acid
{
	class ACID_EXPORT VertexAnimated :
		public IVertex
	{
	public:
		Vector3 m_position;
		Vector2 m_uv;
		Vector3 m_normal;
		Vector3 m_tangent;
		Vector3 m_jointId;
		Vector3 m_vertexWeight;

		VertexAnimated(const Vector3 &position = Vector3::ZERO, const Vector2 &uv = Vector2::ZERO, const Vector3 &normal = Vector3::ZERO, const Vector3 &tangent = Vector3::ZERO, const Vector3 &jointId = Vector3::ZERO, const Vector3 &vertexWeight = Vector3::ZERO);

		VertexAnimated(const VertexAnimated &source);

		~VertexAnimated();

		Vector3 GetPosition() const override { return m_position; };

		void SetPosition(const Vector3 &position) override { m_position = position; };

		size_t GetSize() const override { return sizeof(VertexAnimated); }

		void *GetData(std::vector<IVertex *> &vertices) override;

		static VertexInput GetVertexInput();
	};
}

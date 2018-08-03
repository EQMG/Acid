#pragma once

#include <vector>
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Renderer/Pipelines/PipelineCreate.hpp"
#include "IVertex.hpp"

namespace acid
{
	class ACID_EXPORT VertexModel :
		public IVertex
	{
	public:
		Vector3 m_position;
		Vector2 m_uv;
		Vector3 m_normal;
		Vector3 m_tangent;

		VertexModel(const Vector3 &position = Vector3::ZERO, const Vector2 &uv = Vector2::ZERO, const Vector3 &normal = Vector3::ZERO, const Vector3 &tangent = Vector3::ZERO);

		VertexModel(const VertexModel &source);

		~VertexModel();

		Vector3 GetPosition() const override { return m_position; };

		void SetPosition(const Vector3 &position) override { m_position = position; };

		Vector2 GetUv() const { return m_uv; };

		void SetUv(const Vector2 &uv) { m_uv = uv; };

		Vector3 GetNormal() const { return m_normal; };

		void SetNormal(const Vector3 &normal) { m_normal = normal; };

		Vector3 GetTangent() const { return m_tangent; };

		void SetTangent(const Vector3 &tangent) { m_tangent = tangent; };

		size_t GetSize() const override { return sizeof(VertexModel); }

		void *GetData(std::vector<IVertex *> &vertices) override;

		static VertexInput GetVertexInput();
	};
}

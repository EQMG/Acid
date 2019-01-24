#pragma once

#include <vector>
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Models/IVertex.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
	class ACID_EXPORT VertexAnimated :
		public IVertex
	{
	public:
		VertexAnimated(const Vector3 &position = Vector3::Zero, const Vector2 &uv = Vector2::Zero, const Vector3 &normal = Vector3::Zero, const Vector3 &tangent = Vector3::Zero, const Vector3 &jointId = Vector3::Zero, const Vector3 &vertexWeight = Vector3::Zero);

		const Vector3 &GetPosition() const override { return m_position; };

		void SetPosition(const Vector3 &position) override { m_position = position; };

		const Vector2 &GetUv() const { return m_uv; };

		void SetUv(const Vector2 &uv) { m_uv = uv; };

		const Vector3 &GetNormal() const { return m_normal; };

		void SetNormal(const Vector3 &normal) { m_normal = normal; };

		const Vector3 &GetTangent() const { return m_tangent; };

		void SetTangent(const Vector3 &tangent) { m_tangent = tangent; };

		const Vector3 &GetJointId() const { return m_jointId; };

		void SetJointId(const Vector3 &jointId) { m_jointId = jointId; };

		const Vector3 &GetVertexWeight() const { return m_vertexWeight; };

		void SetVertexWeight(const Vector3 &vertexWeight) { m_vertexWeight = vertexWeight; };

		static Shader::VertexInput GetVertexInput(const uint32_t &binding = 0);
	private:
		Vector3 m_position;
		Vector2 m_uv;
		Vector3 m_normal;
		Vector3 m_tangent;
		Vector3 m_jointId;
		Vector3 m_vertexWeight;
	};
}

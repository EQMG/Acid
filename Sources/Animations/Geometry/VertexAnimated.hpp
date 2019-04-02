#pragma once

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
	VertexAnimated(const Vector3 &position, const Vector2f &uv, const Vector3 &normal, const Vector3 &jointId, const Vector3 &vertexWeight);

	const Vector3 &GetPosition() const override { return m_position; };

	void SetPosition(const Vector3 &position) override { m_position = position; };

	const Vector2f &GetUv() const { return m_uv; };

	void SetUv(const Vector2f &uv) { m_uv = uv; };

	const Vector3 &GetNormal() const { return m_normal; };

	void SetNormal(const Vector3 &normal) { m_normal = normal; };

	const Vector3 &GetJointId() const { return m_jointId; };

	void SetJointId(const Vector3 &jointId) { m_jointId = jointId; };

	const Vector3 &GetVertexWeight() const { return m_vertexWeight; };

	void SetVertexWeight(const Vector3 &vertexWeight) { m_vertexWeight = vertexWeight; };

	bool operator==(const VertexAnimated &other) const;

	bool operator!=(const VertexAnimated &other) const;

	static Shader::VertexInput GetVertexInput(const uint32_t &binding = 0);

private:
	Vector3 m_position;
	Vector2f m_uv;
	Vector3 m_normal;
	Vector3 m_jointId;
	Vector3 m_vertexWeight;
};
}

namespace std
{
template<>
struct hash<acid::VertexAnimated>
{
	size_t operator()(acid::VertexAnimated const &vertex) const noexcept
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vertex.GetPosition());
		acid::Maths::HashCombine(seed, vertex.GetUv());
		acid::Maths::HashCombine(seed, vertex.GetNormal());
		acid::Maths::HashCombine(seed, vertex.GetJointId());
		acid::Maths::HashCombine(seed, vertex.GetVertexWeight());
		return seed;
	}
};
}

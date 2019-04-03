#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "IVertex.hpp"

namespace acid
{
class ACID_EXPORT VertexModel :
	public IVertex
{
public:
	VertexModel(const Vector3f &position, const Vector2f &uv, const Vector3f &normal);

	const Vector3f &GetPosition() const override { return m_position; };

	void SetPosition(const Vector3f &position) override { m_position = position; };

	const Vector2f &GetUv() const { return m_uv; };

	void SetUv(const Vector2f &uv) { m_uv = uv; };

	const Vector3f &GetNormal() const { return m_normal; };

	void SetNormal(const Vector3f &normal) { m_normal = normal; };

	bool operator==(const VertexModel &other) const;

	bool operator!=(const VertexModel &other) const;

	static Shader::VertexInput GetVertexInput(const uint32_t &binding = 0);

private:
	Vector3f m_position;
	Vector2f m_uv;
	Vector3f m_normal;
};
}

namespace std
{
template<>
struct hash<acid::VertexModel>
{
	size_t operator()(acid::VertexModel const &vertex) const noexcept
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vertex.GetPosition());
		acid::Maths::HashCombine(seed, vertex.GetUv());
		acid::Maths::HashCombine(seed, vertex.GetNormal());
		return seed;
	}
};
}

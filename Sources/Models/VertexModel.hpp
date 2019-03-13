#pragma once

#include "IVertex.hpp"
#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
class ACID_EXPORT VertexModel : public IVertex
{
  public:
	explicit VertexModel(const Vector3& position = Vector3::Zero, const Vector2& uv = Vector2::Zero, const Vector3& normal = Vector3::Zero);

	const Vector3& GetPosition() const override
	{
		return m_position;
	};

	void SetPosition(const Vector3& position) override
	{
		m_position = position;
	};

	const Vector2& GetUv() const
	{
		return m_uv;
	};

	void SetUv(const Vector2& uv)
	{
		m_uv = uv;
	};

	const Vector3& GetNormal() const
	{
		return m_normal;
	};

	void SetNormal(const Vector3& normal)
	{
		m_normal = normal;
	};

	bool operator==(const VertexModel& other) const;

	bool operator!=(const VertexModel& other) const;

	static Shader::VertexInput GetVertexInput(const uint32_t& binding = 0);

  private:
	Vector3 m_position;
	Vector2 m_uv;
	Vector3 m_normal;
};
}

namespace std
{
template<>
struct hash<acid::VertexModel>
{
	size_t operator()(acid::VertexModel const& vertex) const noexcept
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vertex.GetPosition());
		acid::Maths::HashCombine(seed, vertex.GetUv());
		acid::Maths::HashCombine(seed, vertex.GetNormal());
		return seed;
	}
};
}

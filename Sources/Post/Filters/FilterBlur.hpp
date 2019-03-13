#pragma once

#include "Maths/Vector2.hpp"
#include "Post/PostFilter.hpp"

namespace acid
{
class ACID_EXPORT FilterBlur : public PostFilter
{
  public:
	enum class Type
	{
		_5 = 5,
		_9 = 9,
		_13 = 13
	};

	explicit FilterBlur(const Pipeline::Stage& pipelineStage, const Vector2& direction, const Type& type = Type::_9);

	void Render(const CommandBuffer& commandBuffer) override;

	const Vector2& GetDirection() const
	{
		return m_direction;
	}

	void SetDirection(const Vector2& direction)
	{
		m_direction = direction;
	}

  private:
	PushHandler m_pushScene;

	Type m_type;
	Vector2 m_direction;
};
}

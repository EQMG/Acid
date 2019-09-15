#pragma once

#include "Maths/Vector2.hpp"
#include "Post/PostFilter.hpp"

namespace acid {
class ACID_EXPORT FilterBlur : public PostFilter {
public:
	enum class Type {
		_5 = 5,
		_9 = 9,
		_13 = 13
	};

	explicit FilterBlur(const Pipeline::Stage &pipelineStage, const Vector2f &direction, const Type &type = Type::_9);

	void Render(const CommandBuffer &commandBuffer) override;

	const Vector2f &GetDirection() const { return m_direction; }
	void SetDirection(const Vector2f &direction) { m_direction = direction; }

private:
	std::vector<Shader::Define> GetDefines(const Type &type);

	PushHandler m_pushScene;

	Type m_type;
	Vector2f m_direction;
};
}

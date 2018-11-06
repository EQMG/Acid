#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	enum BlurType
	{
		BLUR_TYPE_5 = 5,
		BLUR_TYPE_9 = 9,
		BLUR_TYPE_13 = 13
	};

	class ACID_EXPORT FilterBlur :
		public IPostFilter
	{
	private:
		PushHandler m_pushScene;

		BlurType m_blurType;
		Vector2 m_direction;
	public:
		explicit FilterBlur(const GraphicsStage &graphicsStage, const Vector2 &direction, const BlurType &blurType = BLUR_TYPE_9);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		Vector2 GetDirection() const { return m_direction; }

		void SetDirection(const Vector2 &direction) { m_direction = direction; }
	};
}

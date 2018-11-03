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
		UniformHandler m_uniformScene;

		BlurType m_blurType;
		Vector2 m_direction;
	public:
		explicit FilterBlur(const GraphicsStage &graphicsStage, const Vector2 &direction, const BlurType &blurType = BLUR_TYPE_9);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}

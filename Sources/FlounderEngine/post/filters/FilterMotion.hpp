#pragma once

#include "../../Maths/Matrix4.hpp"
#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterMotion :
		public IPostFilter
	{
	private:
		Matrix4 *m_lastViewMatrix;
	public:
		FilterMotion(const int &subpass);

		~FilterMotion();

		void RenderFilter(const VkCommandBuffer *commandBuffer) override;
	};
}

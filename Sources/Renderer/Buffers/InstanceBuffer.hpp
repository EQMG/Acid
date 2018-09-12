#pragma once

#include "Buffer.hpp"

namespace acid
{
	class ACID_EXPORT InstanceBuffer :
		public Buffer
	{
	private:
		VkDescriptorBufferInfo m_bufferInfo;
	public:
		InstanceBuffer(const VkDeviceSize &size);

		void Update(const void *newData);
	};
}

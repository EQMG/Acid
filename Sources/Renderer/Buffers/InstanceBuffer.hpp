#pragma once

#include "Buffer.hpp"

namespace acid
{
	class ACID_EXPORT InstanceBuffer :
		public Buffer
	{
	public:
		explicit InstanceBuffer(const VkDeviceSize &size);

		void Update(const void *newData);
	private:
		VkDescriptorBufferInfo m_bufferInfo;
	};
}

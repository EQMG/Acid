#pragma once

#include "Buffer.hpp"

namespace acid
{
	class ACID_EXPORT InstanceBuffer :
		public Buffer
	{
	public:
		explicit InstanceBuffer(const VkDeviceSize &size);

		void Map(const CommandBuffer &commandBuffer, void **data);

		void Unmap(const CommandBuffer &commandBuffer);

		void Update(const CommandBuffer &commandBuffer, const void *newData);
	};
}

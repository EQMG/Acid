#pragma once

#include "Buffer.hpp"

namespace acid {
class ACID_GRAPHICS_EXPORT InstanceBuffer : public Buffer {
public:
	explicit InstanceBuffer(VkDeviceSize size);

	void Update(const CommandBuffer &commandBuffer, const void *newData);
};
}

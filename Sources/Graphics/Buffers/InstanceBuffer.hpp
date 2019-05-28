﻿#pragma once

#include "Buffer.hpp"

namespace acid
{
class InstanceBuffer :
	public Buffer
{
public:
	explicit InstanceBuffer(const VkDeviceSize &size);

	void Update(const CommandBuffer &commandBuffer, const void *newData);
};
}

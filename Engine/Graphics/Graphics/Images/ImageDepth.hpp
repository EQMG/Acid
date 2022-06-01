#pragma once

#include "Image.hpp"

namespace acid {
/**
 * @brief Resource that represents a depth stencil image.
 */
class ACID_GRAPHICS_EXPORT ImageDepth : public Image {
public:
	explicit ImageDepth(const Vector2ui &extent, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
};
}

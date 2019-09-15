#pragma once

#include "StdAfx.hpp"

namespace acid {
/**
 * @brief Class used to manage {@link Subrender} objects to create a list of render pass.
 */
class ACID_EXPORT Renderer {
public:
	virtual ~Renderer() = default;

	/**
	 * Run when updating the renderer manager.
	 */
	virtual void Update() = 0;
};
}

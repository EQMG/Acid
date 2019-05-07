#pragma once

#include "SubrenderHolder.hpp"

namespace acid
{
class Subrender;

/**
 * Class used to manage {@link Subrender} objects to create a list of render pass.
 */
class ACID_EXPORT Renderer
{
public:
	virtual ~Renderer() = default;

	/**
	 * Run when updating the renderer manager.
	 */
	virtual void Update() = 0;

protected:
	/**
	 * Gets the render holder used by when a renderer is using this manager.
	 * @return The renderer holder.
	 */
	SubrenderHolder &GetRenderHolder() { return m_renderHolder; }

private:
	friend class Graphics;

	SubrenderHolder m_renderHolder;
};
}
